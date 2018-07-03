# KEYFREE smart bike lock - Arduino
This repository details the Arduino development process for the KEEFREE smart bike lock. For further detail please visit the [main repository here.](https://github.com/paulodacaya/keefree_smart_bike_lock)

---

## Table Of Contents
1. [Arduino Compatible Components Used](#one)
2. [Bluetooth Development](#two)
3. [Accelerometer Development](#three)
4. [SMS/CALL Development](#four)
5. [GPS Development](#five)

---

<a name="one"></a>
## Arduino Compatible Components Used
List of Components/Modules used:
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

<a name="two"></a>

## Bluetooth Development

Bluetooth V4.0 BLE will be used for the following reasons:
- Draws low current when sending data (9mA).
- When bluetooth is not paired, draws low current (9mA).
- "AT" commands that uses extremely low current in sleep mode (190microAmps). Helps with our projects low energy focus.
- Pairing speed faster is slightly than bluetooth 2.0.

<a name="three"></a>

## Accelerometer Development

Only the Accelerometer of the MPU6050 Module will be used for the following reasons:

- To trigger alarm and SMS notification for security purposes.
- Triggers only occur when accelerometer constantly reads above a minimum value for a specific duration. Prevents small collisions from activating triggers.

<a name="four"></a>

## GSM SIM800L Development

This module is used to send alerts to KEFREE smart bike lock user. Alerts are in the form of a an SMS or a Call.

<a name="five"></a>

## NEO6MV2 GPS Development

This GPS module is used to determine the location (can also determine speed) of device. It's purposes include the following:

- Determine last known location.
- Send KEFREE location the users device in order to update map.
- Initial sending option is through bluetooth but will use SMS to send location if no signal.
- Android device will capture SMS co-ordinates and display on the app's map.


