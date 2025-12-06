#include "neo_blinky.h"


void neo_blinky(void *pvParameters) {
    static Adafruit_NeoPixel pixels(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    pixels.begin();

    DHT_Data neoData;
    int brightness = 128;

    while(1) {
        // 1. Chờ tín hiệu đồng bộ từ Task DHT
        if (xSemaphoreTake(sem_neo, portMAX_DELAY) == pdTRUE) {
            // 2. Cập nhật brightness nếu có dữ liệu mới trong Queue
            int newBrightness;
            if (qNeoBrightness != nullptr &&
                xQueueReceive(qNeoBrightness, &newBrightness, 0) == pdPASS)
            {
                brightness = newBrightness;
            }
            // 3. Đọc dữ liệu độ ẩm (không xóa khỏi queue)
            if (xQueuePeek(xQueueDHT, &neoData, 0) == pdTRUE) {

                float h = neoData.humidity;
                // 4. Chọn màu theo độ ẩm (giữ logic cũ)
                if (h < 60) {
                    // Xanh lá
                    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
                }
                else if (h < 80) {
                    // Vàng
                    pixels.setPixelColor(0, pixels.Color(255, 255, 0));
                }
                else {
                    // Đỏ
                    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
                }
                // 5. Áp brightness từ Web xuống rồi show
                pixels.setBrightness(brightness); // 0–255
                pixels.show();
            }
        }
    }
}