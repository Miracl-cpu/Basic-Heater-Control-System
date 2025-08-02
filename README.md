# Basic-Heater-Control-System
Temperature-based heater system using ESP32, FreeRTOS, and RGB indicators.
# Heater Control System (ESP32 + FreeRTOS)

This is an embedded system implementation for a basic heater control system using an ESP32 microcontroller, DHT22 sensor, and RGB LED indicators. Built for the upliance.ai internship assignment.

## Features
- Uses DHT22 to read ambient temperature
- Controls a simulated heater (via LED)
- Implements 5 states: IDLE, HEATING, STABILIZING, TARGET_REACHED, OVERHEAT
- RGB LED indicates current heater state
- Uses FreeRTOS with 3 tasks:
  - `sensorTask`: Reads temperature
  - `controlTask`: Determines heater state and controls it
  - `ledTask`: Updates RGB LED color
- Serial Monitor logs temperature and state continuously

  ## C++ Code Files

- `HCS WITH FreeRTOS sketch.ino`: Main implementation using ESP32 + FreeRTOS  
- `HCS without FreeRTOS_sketch.ino`: Non-FreeRTOS version using standard Arduino loop

> You may consider **either of the `.ino` files** as the required C++ code for the assignment submission. The FreeRTOS version is more modular and aligns with bonus features.


## Hardware Connections

| Component        | ESP32 Pin |
|------------------|-----------|
| DHT22 (Data)      | GPIO 15   |
| Heater (LED)      | GPIO 5    |
| RGB LED - Red     | GPIO 12   |
| RGB LED - Green   | GPIO 13   |
| RGB LED - Blue    | GPIO 14   |

## Libraries Required
- DHT sensor library (`DHT.h`)
- FreeRTOS (comes built-in with ESP32 Arduino)

## How to Run
1. Upload code to ESP32 using Arduino IDE or Wokwi.
2. Connect DHT22, RGB LED, and heater LED to specified pins.
3. Open Serial Monitor at 115200 baud rate to view temperature and state logs. (not needed for wokwi)

## Links
* wokwi
- Simulation(with FreeRTOS): [https://wokwi.com/projects/438169427440139265]
- Simulation(without FreeRTOS):[https://wokwi.com/projects/438095549950034945]

