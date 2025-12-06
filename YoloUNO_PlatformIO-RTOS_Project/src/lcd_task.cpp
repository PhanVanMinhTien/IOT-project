#include "lcd_task.h"
#include "temp_humi_monitor.h"
#include "global.h"

LiquidCrystal_I2C lcd(33,16,2);

void lcd_task(void *pvParameters) {

    DHT_Data recvData;

    lcd.begin();
    lcd.backlight();

    while (1) {
        // Đọc dữ liệu mới nhất (không xoá khỏi queue để task khác còn dùng)
        if (xQueuePeek(xQueueDHT, &recvData, portMAX_DELAY) == pdTRUE) {

            float t = recvData.temperature;
            float h = recvData.humidity;

            // ==========================
            // Phân loại trạng thái nhiệt độ
            // ==========================
            const char* tempStateText;
            if (t < 25.0f) {
                tempStateText = "COLD";
            } else if (t < 30.0f) {
                tempStateText = "WARM";
            } else {
                tempStateText = "HOT ";
            }

            // ==========================
            // Phân loại trạng thái độ ẩm
            // ==========================
            const char* humiStateText;
            if (h < 60.0f) {
                humiStateText = "DRY ";
            } else if (h < 80.0f) {
                humiStateText = "NORM";
            } else {
                humiStateText = "WET ";
            }

            // ==========================
            // DÒNG 1: Nhiệt độ + trạng thái nhiệt độ
            // Format: "T:27.5C WARM   "
            // ==========================
            lcd.setCursor(0, 0);
            lcd.print("T:");
            lcd.print(t, 1);        
            lcd.print((char)223);     
            lcd.print("C ");

            lcd.print(tempStateText);

            //
            lcd.print("   ");

            // ==========================
            // DÒNG 2: Độ ẩm + trạng thái độ ẩm
            // Format: "H:65.0% NORM   "
            // ==========================
            lcd.setCursor(0, 1);
            lcd.print("H:");
            lcd.print(h, 1);
            lcd.print("% ");

            lcd.print(humiStateText);
            lcd.print("   ");
        }

        // Cập nhật LCD mỗi 1 giây
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

    

