#include <TinyGPS++.h>    // Library for GPS parsing
#include <WiFi.h>
#include <HTTPClient.h>  

// WiFi Details
const char ssid = "ARRIS-C75C";
const char password = "07A03B13D7C50526";

// GPS Pins
#define RXPin 16
#define TXPin 17

// LED Pin
#define LEDPin 2  // Built-in LED for ESP32 (or any other GPIO pin)

// Create GPS and Serial objects
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// OSM Overpass API URL
String baseURL = "https://overpass-api.de/api/interpreter?data=[out:json];way[maxspeed](around:50,";

// Speed limit variable
int speedLimit = -1;

void setup() {
  Serial.begin(115200);             // Initialize serial monitor
  gpsSerial.begin(9600, SERIAL_8N1, RXPin, TXPin); // GPS serial communication

  pinMode(LEDPin, OUTPUT);          // Set LED pin as output

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  // Process GPS data
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);

    if (gps.location.isUpdated()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      float speed = gps.speed.kmph();  // Speed in km/h

      Serial.print("Latitude: ");
      Serial.print(latitude, 6);
      Serial.print(", Longitude: ");
      Serial.print(longitude, 6);
      Serial.print(", Speed: ");
      Serial.print(speed);
      Serial.println(" km/h");

      // Fetch speed limit from OSM API
      speedLimit = getSpeedLimit(latitude, longitude);

      // Compare speed with speed limit
      if (speedLimit > 0) {
        Serial.print("Speed Limit: ");
        Serial.print(speedLimit);
        Serial.println(" km/h");

        if (speed > speedLimit) {
          digitalWrite(LEDPin, HIGH); // Over-speeding alert
          Serial.println("Over Speeding! Slow Down!");
        } else {
          digitalWrite(LEDPin, LOW);  // Speed is within the limit
        }
      } else {
        Serial.println("Speed limit not available");
      }
    }
  }
}

int getSpeedLimit(float latitude, float longitude) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construct the API query
    String query = baseURL + String(latitude, 6) + "," + String(longitude, 6) + ");out;";
    Serial.println("Querying OSM API: " + query);

    http.begin(query);

    // Send GET request
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("OSM Response: " + response);

      // Parse speed limit from JSON response (simplified for demo)
      int speedLimitIndex = response.indexOf("\"maxspeed\":\"");
      if (speedLimitIndex != -1) {
        String speedLimitStr = response.substring(speedLimitIndex + 11, response.indexOf("\"", speedLimitIndex + 11));
        http.end();
        return speedLimitStr.toInt();
      }
    } else {
      Serial.println("HTTP Error: " + String(httpResponseCode));
    }
    http.end();
  }
  return -1; // Return -1 if speed limit is not found
}
