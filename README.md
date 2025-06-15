# ESP32-RFIDSmartDoorLock
A tutorial for building RFID door locks using the ESP32 module.
Added some Blynk integration for features like real-time notification, registering card, and manually opening the door.
This project can be re-registered using Blynk IoT push button.
Registered card are kept in EEPROM.

Also, this is a project for final assignment of Microcontroller. So, be free to use this code.

# Stuff you need
- **ESP32 (prefer Devkit V1)**
- 2x **12V Adaptor (atleast 1A of current)**
- 2x **12V to 5V Stepdown Converter (MP1584)**
- **RC-522 RFID Reader**
- **2 Channel Relay (make sure it is compatible with 3.3V voltage)**
- **Magnetic Reed Switch**
- **2.2K ohm resistor**
- **Piezo (recommend to use SFM-27)**
- **12V Solenoid Lock**

# Wiring Diagram
This are the wiring diagram for the project.

![Screenshot](wiring.png)

- **From ESP32 to RFID Reader**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/rfidwiring.png)
