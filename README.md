IoT Application Development – Final Project

ESP32-S3 YoloUNO • FreeRTOS • TinyML • Web Dashboard • MQTT Cloud Integration

This repository contains the full implementation of my final project for the IoT Application Development course.
The project extends the base YoloUNO PlatformIO RTOS template to build a complete real-time IoT system including sensing, control, web dashboard UI, cloud telemetry, and TinyML anomaly detection.

⭐ Project Overview

The system is implemented on an ESP32-S3 YoloUNO board, using multiple FreeRTOS tasks to manage:

Environmental sensing (temperature + humidity via DHT20)

LED visual feedback

NeoPixel color visualization

LCD screen local display

WiFi management (AP ↔ STA mode)

Web-based dashboard with WebSocket real-time updates

MQTT telemetry publishing to CoreIoT cloud

TinyML inference for anomaly detection

A strict no global variables requirement is addressed by using FreeRTOS Queues and Binary Semaphores for inter-task communication.

📦 Features Implemented (6 Tasks)
✔ Task 1 – LED Control (Temperature-based)

LED brightness & blinking patterns depend on temperature range:

Cool → steady light

Warm → slow blink

Hot → fast strobe

Uses PWM (LEDC) instead of digitalWrite.

Triggered by semaphore → zero CPU polling.

✔ Task 2 – NeoPixel Control (Humidity-based)

Humidity mapped to clear RGB states:

Green (< 60%): Good

Yellow (60–80%): Humid

Red (> 80%): High moisture

Brightness adjustable via Web Dashboard slider.

Synchronized using FreeRTOS semaphores.

✔ Task 3 – LCD Monitor Display

Displays both:

Temperature + Temperature Status (Cool / Normal / Hot)

Humidity + Humidity Status (Low / Normal / High)

Updated every 1 second via queue-peek architecture.

✔ Task 4 – Web Interface + Real-Time Dashboard

ESP32 hosts a local Web Dashboard using:

HTML + CSS + JavaScript

WebSocket for instant sensor updates

Dashboard includes:

Live temperature/humidity cards

Color-coded status indicators

Relay manager UI (add/delete relays)

Clean UI with responsive cards.

✔ Task 5 – TinyML Anomaly Detection

Uses a pretrained TensorFlow Lite Micro model (dht_anomaly_model.h).

Input: 2 features (temperature, humidity)

Output: anomaly score / predicted class

Runs every 5 seconds inside its own RTOS task.

Integrated with FreeRTOS queues for latest sensor data.

✔ Task 6 – MQTT Cloud Telemetry (CoreIoT Platform)

Publishes environmental data every 10 seconds.

MQTT topic example:

esp/telemetry


Receives remote RPC commands (e.g., LED toggle).

Automatically reconnects when WiFi/MQTT drops.

🧱 System Architecture
Hardware Block Diagram
 ┌─────────────┐       I2C        ┌──────────────┐
 │   ESP32-S3  │ ───────────────► │   DHT20      │
 │   YoloUNO   │                  └──────────────┘
 │             │       I2C        ┌──────────────┐
 │             │ ───────────────► │  LCD1602     │
 │             │                  └──────────────┘
 │             │      GPIO        ┌──────────────┐
 │             │ ───────────────► │ NeoPixel RGB │
 │             │                  └──────────────┘
 │             │      GPIO        ┌──────────────┐
 │             │ ───────────────► │ Built-in LED │
 └─────────────┘                  └──────────────┘

RTOS Task Interaction Diagram
          ┌────────────────────┐
          │ temp_humi_monitor  │
          │ (Producer)         │
          └──────┬─────────────┘
                 │ Queue + Semaphores
     ┌───────────┼────────────┬───────────────┐
     ▼           ▼            ▼               ▼
┌────────┐  ┌──────────┐  ┌────────┐   ┌────────────────┐
│ LED    │  │ NeoPixel │  │ LCD     │   │ TinyML        │
│ Task   │  │ Task      │ │ Task    │   │ Task          │
└────────┘  └──────────┘  └────────┘   └────────────────┘
                        │
                        ▼
                ┌──────────────┐
                │ CoreIoT Task │
                │ (MQTT Cloud) │
                └──────────────┘

🌐 Web Dashboard Preview
Home Dashboard

(Insert your screenshot here)

Temperature & Humidity Cards

(Insert second screenshot)

Relay Management UI

(Insert third screenshot)

🚀 How to Build & Upload
1. Requirements

PlatformIO

ESP32-S3 YoloUNO board

Python 3 (for PlatformIO)

2. Build & Flash
pio run
pio run --target upload
pio device monitor

📁 Project Structure
├── src/
│   ├── main.cpp
│   ├── temp_humi_monitor.cpp
│   ├── led_blinky.cpp
│   ├── neo_blinky.cpp
│   ├── lcd_task.cpp
│   ├── task_webserver.cpp
│   ├── coreiot.cpp
│   ├── tinyml.cpp
│   └── ...
├── data/
│   ├── index.html
│   ├── script.js
│   └── styles.css
├── include/
│   ├── *.h
├── platformio.ini
├── README.md
└── ...

🧪 Experiment Summary

All 6 required tasks were completed:

Task	Status	Notes
Task 1	✔ Done	LED PWM control, 3 temperature levels
Task 2	✔ Done	NeoPixel RGB logic + brightness queue
Task 3	✔ Done	LCD two-line status display
Task 4	✔ Done	Full Web dashboard + WebSocket live update
Task 5	✔ Done	TinyML inference using pretrained model
Task 6	✔ Done	MQTT telemetry + RPC handling
📌 Conclusion

This project demonstrates a complete IoT system running entirely on FreeRTOS with real-time sensing, multi-modal user interaction, a modern Web UI, TinyML edge inference, and cloud integration.
It successfully implements all required tasks, achieving more than 30% redesign of the base template.

👤 Author

Phan Văn Minh Tiến
Computer Engineering – HCMUT
Student ID: 2153888
