#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

String ssid = "Happy New Year 2050";
String password = "12345678";
String wifi_ssid = "Happy New Year 2050";
String wifi_password = "12345678";
boolean isWifiConnected = false;


SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t xTempSemaphore = xSemaphoreCreateBinary();
SemaphoreHandle_t xHumiSemaphore = xSemaphoreCreateBinary();

  // 1) Tạo semaphore cho 3 mức nhiệt độ
  SemaphoreHandle_t xTempSemNormal   = xSemaphoreCreateBinary();
  SemaphoreHandle_t xTempSemWarning  = xSemaphoreCreateBinary();
  SemaphoreHandle_t xTempSemCritical = xSemaphoreCreateBinary();
  // 2) Tạo semaphore cho 3 mức độ ẩm
  SemaphoreHandle_t xHumiSemNormal   = xSemaphoreCreateBinary();
  SemaphoreHandle_t xHumiSemWarning  = xSemaphoreCreateBinary();
  SemaphoreHandle_t xHumiSemCritical = xSemaphoreCreateBinary();

QueueHandle_t xQueueDHT = xQueueCreate(1, sizeof(DHT_Data));
SemaphoreHandle_t sem_led = xSemaphoreCreateBinary();
SemaphoreHandle_t sem_neo = xSemaphoreCreateBinary();

QueueHandle_t qBuiltinBrightness = xQueueCreate(1, sizeof(int));
QueueHandle_t qNeoBrightness     = xQueueCreate(1, sizeof(int));