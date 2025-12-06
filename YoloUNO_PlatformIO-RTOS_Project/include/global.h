#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern float glob_temperature;
extern float glob_humidity;

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern String ssid;
extern String password;
extern String wifi_ssid;
extern String wifi_password;
extern boolean isWifiConnected;

extern SemaphoreHandle_t xBinarySemaphoreInternet;


// 1. Định nghĩa gói dữ liệu
struct DHT_Data {
    float temperature;
    float humidity;
};
// 2. Khai báo Handle cho Queue (thay vì chỉ dùng Semaphore)
extern QueueHandle_t xQueueDHT; 

// Các Semaphore cũ (giữ lại nếu cần kích hoạt sự kiện khẩn cấp, nhưng Queue quan trọng hơn)
extern SemaphoreHandle_t sem_led; 
extern SemaphoreHandle_t sem_neo;

//
extern SemaphoreHandle_t xTempSemaphore;
extern SemaphoreHandle_t xHumiSemaphore;

extern SemaphoreHandle_t xTempSemNormal;
extern SemaphoreHandle_t xTempSemWarning;
extern SemaphoreHandle_t xTempSemCritical;

extern SemaphoreHandle_t xHumiSemNormal;
extern SemaphoreHandle_t xHumiSemWarning;
extern SemaphoreHandle_t xHumiSemCritical;

extern QueueHandle_t qBuiltinBrightness;
extern QueueHandle_t qNeoBrightness;

#endif