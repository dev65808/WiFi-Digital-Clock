# ESP32 + Arduino Nano: Real-Time Environment Display (CO, Temp, Humidity, Time)

This project uses an ESP32 and an Arduino Nano to create a real-time environmental monitoring display. It collects temperature, humidity, CO level, and time, and displays them on a 64x16 LED matrix using the DMD2 library.

## Components Used

- ESP32: Connects to Wi-Fi and syncs time via NTP. It reads sensor data and sends it to Arduino Nano via Serial.

- Arduino Nano: Receives and parses data from ESP32 and manages the display.

- SMPS(5v & 5A): It is used for brightning the p10

- 64x16 LED Dot Matrix Display (2 × 32x16 panels)

- DHT11 Sensor: For temperature and humidity

- MQ-7 Sensor: For CO detection

- DS3231 RTC: For accurate timekeeping (with NTP sync fallback)

- Push Button: For resetting Wi-Fi credentials

- WiFiManager: For easy Wi-Fi setup on first boot

## System Overview

### ESP32

- Connects to Wi-Fi (using captive portal on first boot).

- Syncs time using NTP and stores it in DS3231 RTC.

- Every 1 second: Sends current time (e.g., 23:56:10) to Arduino Nano.

- Every 5 seconds: Sends sensor data (e.g., T:26.5 H:53 CO:385) to Arduino Nano.

### Arduino Nano

- Parses incoming serial data from ESP32.

Displays:

- Time (when received)

- Temperature & Humidity (in one view)

- CO Level (in another view)

- Automatically switches between views every few seconds.

## 📁 Project Structure

<pre> <code> 
📂 /ESP32
 └── esp32_main.ino         # Handles Wi-Fi, NTP, RTC, sensor reading, and serial output

📂 /Nano
 └── nano_display.ino       # Parses serial input and displays on DMD
</code> </pre>

📜 README.md                # Project overview (this file)

## Features

- Wi-Fi provisioning with captive portal (WiFiManager)

- Accurate time via NTP with fallback to RTC

- Real-time sensor readings (Temp, Humidity, CO)

- Stable, smooth time updates without jumps or lags

- Clean font display switching for clarity

- Simple serial protocol for reliable communication

## Demo

![Waiting](WiFi%20Digital%20Clock/Images/Waiting.jpg)
![Time](WiFi%20Digital%20Clock/Images/Time.jpg)
![Temperature and Humidity](WiFi%20Digital%20Clock/Images/Temp+Humidity.jpg)
![CO Level](WiFi%20Digital%20Clock/Images/CO%20level.jpg)
![Circuit Diagram](WiFi%20Digital%20Clock/Images/circuit_image.png)

## Setup Instructions

- Flash esp32_main.ino to ESP32.

- Flash nano_display.ino to Arduino Nano.

- Power on ESP32 → Connect to ESP-Clock Wi-Fi AP → Enter your Wi-Fi credentials.

- Watch the matrix start displaying synced time and sensor data.
