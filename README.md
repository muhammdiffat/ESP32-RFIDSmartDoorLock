# ESP32-RFIDSmartDoorLock
A tutorial for building RFID door locks using ESP32 module.
Added some Blynk integration for features like real-time notification, registering or re-registering card, and manually opening the door.
Registered card are kept in EEPROM.

IDE used for this project is Arduino IDE.

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
These are the wiring diagram for the project.

![Screenshot](wiring.png)


- **From ESP32 to RFID Reader**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/rfidwiring.png)


- **Buzzer/Piezo**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/piezoWiring.png)


- **Magnetic Reed Switch**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/reedswitchWiring.png)


- **Relay Circuitry Wiring**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/RelayWiring.png)


- **Solenoid Lock Wiring**

![Screenshot](https://github.com/muhammdiffat/ESP32-RFIDSmartDoorLock/blob/main/Wiring%20Diagrams/solenoidWiring.png)


# More information about this project..

**Q**: Why did you use different/external power adapter? Why not combine it as one?

**A**: From my testing, the solenoid somehow interfering with the electromagnetic field of the RFID reader, it only can be used 1 time. After that the RFID reader could not work, unless resetting the ESP32.
