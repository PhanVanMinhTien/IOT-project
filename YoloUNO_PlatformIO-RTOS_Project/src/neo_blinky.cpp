#include "neo_blinky.h"


void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    while(1) {                          
        // Chờ đến khi có humidity mới
        if (xSemaphoreTake(xHumiSemaphore, portMAX_DELAY) == pdTRUE) {
            float h = glob_humidity;

            if (h < 50) {
                strip.setPixelColor(0, strip.Color(0, 0, 40)); // Blue
            } else if (h >= 50 && h < 70) {
                strip.setPixelColor(0, strip.Color(0, 40, 0)); // Green
            } else {
                strip.setPixelColor(0, strip.Color(40, 0, 0)); // Red
            }
            strip.show(); // Hiển thị màu theo độ ẩm

            // Giữ màu 2 giây (chỉ block task này, không block hệ thống)
            vTaskDelay(2000);

            // Tắt LED sau 2 giây
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
        }
    }
}
