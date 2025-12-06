# IoT Application Development – Final Project  
### ESP32-S3 YoloUNO • FreeRTOS • Web Dashboard • TinyML • CoreIoT Cloud  
**Authors:** Phan Văn Minh Tiến – 2153888, Nguyễn Thành Đạt – 2052323  
**Instructor:** Dr. Nguyễn Văn Vinh  
**Course:** CO3037 – IoT Application Developing – HCMUT  

---

## 📌 Project Overview  
This project extends the original **YoloUNO PlatformIO RTOS template** by implementing a fully functional multi-task IoT monitoring system using **FreeRTOS**, **WebSocket Dashboard**, **MQTT Cloud Telemetry**, and **TinyML anomaly detection**.

The system reads temperature & humidity from a DHT20 sensor, controls visual indicators (LED & NeoPixel), displays information on LCD, serves a real-time dashboard via WiFi, and publishes telemetry to the **CoreIoT cloud**.  
A lightweight TensorFlow Lite model is also embedded for anomaly detection.

---

## 📂 Folder Structure


---

## 🚀 Features Implemented (Six Tasks)

### **Task 1 – LED Control Based on Temperature**
- Event-driven LED update using **binary semaphore**.  
- Smooth PWM brightness control using LEDC.  
- Three temperature states: *Cool*, *Warm*, *Hot*.

---

### **Task 2 – NeoPixel Color Based on Humidity**
- Uses `Adafruit_NeoPixel`.  
- Triggered by semaphore → no busy-waiting.  
- Color mapping: Green (Dry), Yellow (Humid), Red (High Humidity).

---

### **Task 3 – LCD Display Task**
- Displays both temperature & humidity with **three-state descriptors**.  
- Uses `xQueuePeek()` to avoid removing data from shared queue.

---

### **Task 4 – Web Dashboard (AP Mode)**
- Built using **ESPAsyncWebServer + WebSocket + LittleFS**.  
- Real-time temperature/humidity dashboard.  
- UI includes:
  - Sensor cards with color-coded alert levels.
  - Relay control interface (add, display, manage relays).
  - Settings page for WiFi & CoreIoT configuration.

---

### **Task 5 – TinyML Anomaly Detection**
- Embedded TensorFlow Lite Micro model.  
- Input: Temperature + Humidity.  
- Output: 1-node anomaly score.  
- Called every 5 seconds inside `tiny_ml_task`.

---

### **Task 6 – CoreIoT Cloud Telemetry**
- MQTT publishing of telemetry JSON:
```json
{
  "temperature": <value>,
  "humidity": <value>
}
