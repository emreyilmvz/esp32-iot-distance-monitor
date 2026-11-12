# ESP32 IoT Distance Monitoring and Alert System

An ESP32 and HC-SR04 measure distance and publish readings to the MQTT topic `ev/mesafe`. Messages on `ev/led` control the board's built-in LED.

## Hardware and software

- ESP32 board with Arduino core
- HC-SR04 ultrasonic distance sensor
- MQTT broker with a dedicated username and password
- Arduino IDE
- PubSubClient Arduino library

## Setup

1. Copy `firmware/ESP32_Alert_IOT/secrets.example.h` to `secrets.h` in the same folder.
2. Set your Wi-Fi and MQTT broker credentials in `secrets.h`. Do not commit this file.
3. Open `firmware/ESP32_Alert_IOT/ESP32_Alert_IOT.ino` in Arduino IDE and install PubSubClient.
4. Connect HC-SR04 TRIG to GPIO 25 and ECHO to GPIO 26. HC-SR04 ECHO is commonly 5V; ESP32 GPIO pins are 3.3V, so use an appropriate level shifter or voltage divider.
5. Upload the sketch to the ESP32.

## MQTT topics

- Publishes distance in centimeters to `ev/mesafe` when the reading changes by more than 2 cm.
- Subscribes to `ev/led`: payload `0` turns the built-in LED on; payload `1` turns it off.

## Security

Use a dedicated MQTT account with permissions limited to `ev/mesafe` publish and `ev/led` subscribe. The current sketch uses plain MQTT on port 1883, which does not encrypt credentials or messages. Use it only on a trusted, isolated LAN. For untrusted networks or internet access, configure TLS before connecting. Do not expose an unauthenticated broker to the internet.

`secrets.h` is excluded by `.gitignore`. If credentials were ever committed or shared, rotate them; deleting a file in a later commit does not erase it from Git history.

## License

MIT. See [LICENSE](LICENSE).
