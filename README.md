# Speed-limit-monitoring-with-gps-sensor-openstreet-map-API


Project Anaylysis
ESP32: -Reads GPS data from the GY-NEO6M sensor.
      -Extracts the car's real-time speed and location (latitude and longitude).
       -Uses WiFi to query the OpenStreetMap (OSM) Overpass API for the speed limit based on the location.
       -Compares the car's speed with the speed limit.
      -Activates an LED if the speed exceeds the limit. 


COMPONENTS PINS CONNECTION
ESP32 Pins  - GPS Module pins -  Description
RX (GPIO 16)	-    TX	          -GPS Module TX (Output)
TX (GPIO 17)	-     RX	        -GPS Module RX (Input)
3.3V	        -   VCC	          -Power (3.3V or 5V)
GND          -  	GND	          -Ground
GPIO 2	     -     LED	        -Over-speeding alert


Testing
Set Up Hardware: Connect the GPS module to the ESP32 as per the connections table.
Install Libraries: TinyGPS++: Go to Arduino IDE → Tools → Manage Libraries, search for TinyGPS++, and install it.
Write the sketch: Load the written code onto the ESP32 using the Arduino IDE.
OpenStreetMap Overpass API: integrate this API into the sketch
Serial Monitor: Open the Serial Monitor at 115200 baud to observe GPS data and the fetched speed limits.
LED Alert: If the car's speed exceeds the speed limit, the LED should light up

Goal of this Project
-Provide real-time alerts to drivers if their speed exceeds the legal speed limit for a given location.
-Demonstrate the integration of IoT technologies (GPS, WiFi-enabled ESP32, and online APIs) to develop a practical and intelligent system for automotive applications.
