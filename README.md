# ESP32-BLE-Air-Mouse
A wireless air mouse built with an ESP32, MPU6050 gyroscope, and tactile buttons over BLE.

# ESP32 BLE Air Mouse with MPU6050

This project transforms an ESP32 and an MPU6050 gyroscope into a wireless Bluetooth Low Energy (BLE) air mouse. By physically moving the breadboard in the air, you can control the mouse cursor on any Bluetooth-enabled PC or device.

### Components Used:
* ESP32 Microcontroller 
* MPU6050 (6-axis Accelerometer and Gyroscope)
* 2x Tactile Push Buttons (Left & Right Click)
* Breadboard & Jumper wires

### Wiring Pinout:

| Component Pin | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **MPU SDA** | Pin 8 | I2C Data |
| **MPU SCL** | Pin 9 | I2C Clock |
| **MPU VCC** | 3.3V | Power |
| **MPU GND** | GND | Common Ground |
| **Left Button** | Pin 3 & GND | Left Click (Triggers `LOW` on press) |
| **Right Button**| Pin 10 & GND | Right Click (Triggers `LOW` on press) |

## Software Requirements
You will need the Arduino IDE with the ESP32 board manager installed.

**Required Libraries:**
* `Wire.h` (Built into Arduino core)
* `HijelHID_BLEMouse.h` (Ensure this library is installed or included in your sketch directory)

## How It Works
1. **Motion Tracking:** The MPU6050 reads raw gyroscope data (`gx`, `gy`, `gz`). 
2. **Smoothing:** The raw data is passed through a basic exponential moving average filter (`smoothed_dx = smoothed_dx * 0.7 + dx * 0.3`) to prevent erratic cursor jumping.
3. **Deadzone:** A minimal threshold is applied (`abs(dx) < 0.5`) to prevent cursor drift when the device is resting.
4. **Bluetooth HID:** The ESP32 presents itself as a standard Bluetooth Mouse, meaning no custom drivers are required on the host computer.


## Required Packages / Libraries
To compile this project, install the following libraries in your Arduino IDE:

1. **Wire.h**: Built-in I2C library (no installation required).
2. **HijelHID_BLEMouse**: [Add a brief note here on where to find this library, e.g., "Search for ESP32 BLE Mouse in the Library Manager" or paste the link to the library].
