# IoT Application Development - RTOS Project on Yolo Uno ESP32

> **Course:** Internet of Things Application Development (CO3039)
> **Institution:** Ho Chi Minh City University of Technology (HCMUT)
> **Advisor:** Dr. Le Trong Nhan

---

## 📖 Overview

This project builds a comprehensive IoT system on the **ESP32-S3 YoloUNO** platform, utilizing **FreeRTOS** for modular multi-tasking management. The system integrates environmental monitoring (Temperature & Humidity), local alerts via LED/LCD, on-device anomaly detection using **TinyML**, and remote monitoring via a Web Dashboard (WebSocket) and Cloud (CoreIoT/MQTT).

The project strictly adheres to the requirement of **eliminating global variables**, utilizing RTOS **Queues** and **Semaphores** for thread-safe data synchronization.

## 🛠️ Hardware & Software Ecosystem

### Hardware
* **MCU Board:** Yolo Uno (ESP32-S3)
* **Sensor:** DHT20 (Digital Temperature & Humidity - I2C)
* **Display:** LCD 1602 (I2C)
* **Indicators:** NeoPixel RGB LED (GPIO 45), Built-in LED (GPIO 48)

### Software & Libraries
* **Framework:** Arduino Core on PlatformIO
* **OS:** FreeRTOS (Task scheduling, IPC)
* **Connectivity:** WiFi (AP & STA Modes), ESPAsyncWebServer (WebSocket), PubSubClient (MQTT)
* **AI/ML:** TensorFlow Lite Micro (TinyML)
* **Web UI:** HTML5, CSS3, JavaScript (Real-time Dashboard)

---

## 🚀 Key Features (Tasks)

The system consists of 6 main Tasks running in parallel:

### 1. Temperature-Based LED Alert (Task 1)
Controls the Built-in LED to indicate temperature levels:
* **Cool (< 25°C):** LED stays ON (stable).
* **Warning (25°C ≤ T < 30°C):** Slow blinking (1Hz).
* **Critical (≥ 30°C):** Rapid strobe alert (10Hz).
* *Brightness is adjustable via the Web Dashboard.*

### 2. Humidity-Based NeoPixel Indicator (Task 2)
The RGB LED changes color based on humidity ranges:
* 🟢 **Dry / Normal (< 60%):** Green.
* 🟡 **Humid (60% ≤ H < 80%):** Yellow.
* 🔴 **High Moisture (≥ 80%):** Red.

### 3. LCD Environmental Display (Task 3)
Displays both numerical values and qualitative states on the LCD:
* **Line 1:** Temperature value + State (COLD, WARM, HOT).
* **Line 2:** Humidity value + State (DRY, NORM, WET).

### 4. Web Dashboard & WiFi Management (Task 4)
* **Dual Mode (AP/STA):** Automatically starts in AP Mode (`YoloUno_AP`) for configuration if no credentials are found. Switches to Station Mode (STA) after connecting to WiFi.
* **Storage:** WiFi credentials and Cloud Token are saved in **LittleFS** (`/info.dat`).
* **Dashboard:** A responsive Web Interface for real-time monitoring (via **WebSocket**), device control (Relays), and LED brightness adjustment.

### 5. TinyML Anomaly Detection (Task 5)
* Integrates a **TensorFlow Lite Micro** model directly on the ESP32.
* Runs inference every 5 seconds to detect anomalies in sensor data without relying on the cloud.

### 6. Cloud Telemetry - CoreIoT (Task 6)
* Connects to the **CoreIoT** platform via **MQTT**.
* Publishes JSON payloads containing temperature and humidity data every 10 seconds (Topic: `esp/telemetry`).
* Allows for remote historical data visualization.

---

## ⚙️ Installation & Usage

### 1. Prerequisites
* Visual Studio Code
* Extension: **PlatformIO IDE**

### 2. Installation
1.  Clone this repository:
    ```bash
    git clone [https://github.com/PhanVanMinhTien/IOT-project.git](https://github.com/PhanVanMinhTien/IOT-project.git)
    ```
   
2.  Open the project folder in PlatformIO.
3.  Wait for PlatformIO to download necessary dependencies (defined in `platformio.ini`).
4.  Build and Upload the code to your Yolo Uno board.

### 3. Initial Configuration
1.  On first boot (or if WiFi is unavailable), the device broadcasts a WiFi AP named `YoloUno_AP`.
2.  Connect to this WiFi and visit `192.168.4.1` in your browser.
3.  Enter your WiFi SSID, Password, and CoreIoT Token.
4.  Click **Save**. The device will reboot and connect to the internet.
