# KEYFREE smart bike lock - Arduino
This repository details the Arduino development process for the KEEFREE smart bike lock. For further detail please visit the [main repository here.](https://github.com/paulodacaya/keefree_smart_bike_lock)

## Table Of Contents
1. [Arduino Compatible Components Used](#one)
2. [Bluetooth Development](#two)
3. 

---

## Arduino Compatible Components Used<a name="one"></a>
List of Components/Modules used.
- Boards
  - Arduino UNO - Prototyping and Testing
  - Arduino MEGA - Integration
- Modules
  - Bluetooth V4.0 BLE (HM-10)
  - MPU6050 Gyro and Accelerometer 
  - SIM800L Module (GSM GPRS SMS) 
  - NEO6MV2 GPS
- Components
  - 12V DC Solenoid Lock


## Bluetooth Development<a name="two"></a>

Bluetooth V4.0 BLE will be used for the following reasons:
  - Draws low current when sending data (9mA).
  - When bluetooth is not paired, draws low current (9mA).
  - "AT" commands that uses extremely low current in sleep (190microAmps). Helps with our projects low energy focus.
  - Pairing speed faster is slightly than bluetooth 2.0.

