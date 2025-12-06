#include "led_blinky.h"
#include "temp_humi_monitor.h"

enum Led_Blinky_State { LED_IDLE, LED_ON, LED_OFF };
Led_Blinky_State led_blinky_State = LED_IDLE;


void led_blinky(void *pvParameters){
    // Cấu hình PWM cho LED (thay vì digitalWrite)
    const int LED_CHANNEL = 0;
    ledcSetup(LED_CHANNEL, 5000, 8);        // 5 kHz, 8-bit
    ledcAttachPin(LED_GPIO, LED_CHANNEL);

    DHT_Data led_data;
    int brightness = 128;                   // giá trị mặc định khi chưa có gì từ Web

    while(1){
        // 1. Chờ tín hiệu từ Semaphore (Task DHT gọi dậy)
        if (xSemaphoreTake(sem_led, portMAX_DELAY) == pdTRUE) {

            // 2. Nếu có brightness mới trong Queue → cập nhật
            //    xQueueReceive với timeout 0 để không block
            int newBrightness;
            if (qBuiltinBrightness != nullptr &&
                xQueueReceive(qBuiltinBrightness, &newBrightness, 0) == pdPASS)
            {
                brightness = newBrightness;
            }

            // 3. Đọc dữ liệu DHT (không xóa khỏi queue để task khác còn dùng)
            if (xQueuePeek(xQueueDHT, &led_data, 0) == pdTRUE) {

                float t = led_data.temperature;

                // ====== TRẠNG THÁI 1: Mát (< 25) → LED sáng tĩnh ======
                if (t < 25) {
                    ledcWrite(LED_CHANNEL, brightness);
                    // Không delay, giữ nguyên sáng cho đến lần cập nhật kế
                }

                // ====== TRẠNG THÁI 2: 25 <= T < 30 → Nháy chậm ======
                else if (t < 30) {
                    ledcWrite(LED_CHANNEL, brightness);
                    vTaskDelay(500 / portTICK_PERIOD_MS);

                    ledcWrite(LED_CHANNEL, 0);
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                }

                // ====== TRẠNG THÁI 3: >= 30 → Nháy nhanh (5 nhịp) ======
                else {
                    for (int i = 0; i < 5; i++) {
                        ledcWrite(LED_CHANNEL, brightness);
                        vTaskDelay(100 / portTICK_PERIOD_MS);

                        ledcWrite(LED_CHANNEL, 0);
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                    }
                }
            }
        }
        // Không cần delay cuối vòng while:
        // task ngủ lại nhờ xSemaphoreTake(portMAX_DELAY)
    }
}


// void led_blinky(void *pvParameters){
//     // Cấu hình chân LED
//     pinMode(LED_GPIO, OUTPUT);
//     DHT_Data led_data;

//     while(1){
//         // 1. Chờ tín hiệu từ Semaphore (Task DHT gọi dậy)
//         // Dùng portMAX_DELAY để ngủ hoàn toàn cho đến khi có tín hiệu (tiết kiệm CPU)
//         if (xSemaphoreTake(sem_led, portMAX_DELAY) == pdTRUE) {
            
//             // 2. Dùng xQueuePeek để KHÔNG XÓA dữ liệu (để LCD/Cloud còn đọc được)
//             if (xQueuePeek(xQueueDHT, &led_data, 0) == pdTRUE) {
                
//                 // --- Xử lý 3 hành vi nháy đèn khác nhau (Task 1 Yêu cầu) ---
                
//                 // TRẠNG THÁI 1: Mát mẻ (< 25) -> Đèn sáng tĩnh (hoặc tắt hẳn tùy bạn)
//                 if (led_data.temperature < 25) {
//                     digitalWrite(LED_GPIO, HIGH); // Sáng báo hiệu hệ thống OK
//                     // Không delay, giữ nguyên trạng thái chờ lần đọc sau
//                 } 
                
//                 // TRẠNG THÁI 2: Cảnh báo nhẹ (25 <= Temp < 30) -> Nháy Chậm
//                 else if (led_data.temperature < 30) {
//                      // Nháy 1 nhịp chậm (kéo dài khoảng 1s)
//                      digitalWrite(LED_GPIO, HIGH);
//                      vTaskDelay(500 / portTICK_PERIOD_MS);
//                      digitalWrite(LED_GPIO, LOW);
//                      vTaskDelay(500 / portTICK_PERIOD_MS);
//                 } 
                
//                 // TRẠNG THÁI 3: Nguy hiểm (>= 30) -> Nháy Nhanh (Cấp cứu)
//                 else {
//                     // Nháy 5 lần thật nhanh để gây chú ý
//                     for(int i=0; i<5; i++){
//                         digitalWrite(LED_GPIO, HIGH);
//                         vTaskDelay(100 / portTICK_PERIOD_MS);
//                         digitalWrite(LED_GPIO, LOW);
//                         vTaskDelay(100 / portTICK_PERIOD_MS);
//                     }
//                 }
//             }
//         }
//         // Lưu ý: Không cần vTaskDelay(1000) ở cuối loop nữa 
//         // vì xSemaphoreTake đã đóng vai trò chờ đợi rồi.
//         // Task sẽ tự ngủ cho đến khi Task Read DHT gửi tín hiệu tiếp theo (sau 2s).
//     }
// }





// void led_blinky(void *pvParameters){
//   pinMode(LED_GPIO, OUTPUT);

//   // State hiện tại của LED (0: normal, 1: warning, 2: critical)
//   int ledState = 0;

//   while(1) {

//     // Kiểm tra xem có tín hiệu state mới nào không
//     if (xSemaphoreTake(xTempSemNormal, 0) == pdTRUE) {
//         ledState = 0;
//     }
//     if (xSemaphoreTake(xTempSemWarning, 0) == pdTRUE) {
//         ledState = 1;
//     }
//     if (xSemaphoreTake(xTempSemCritical, 0) == pdTRUE) {
//         ledState = 2;
//     }

//     // Chọn pattern nháy theo ledState
//     int blinkTotal = 0;
//     int onDelay = 0;
//     int offDelay = 0;

//     switch (ledState) {
//         case 0: // NORMAL
//             blinkTotal = 2;
//             onDelay  = 800;
//             offDelay = 800;
//             break;
//         case 1: // WARNING
//             blinkTotal = 4;
//             onDelay  = 400;
//             offDelay = 400;
//             break;
//         case 2: // CRITICAL
//         default:
//             blinkTotal = 8;
//             onDelay  = 150;
//             offDelay = 150;
//             break;
//     }

//     // Thực hiện pattern
//     for (int i = 0; i < blinkTotal; i++) {
//         digitalWrite(LED_GPIO, HIGH);
//         vTaskDelay(pdMS_TO_TICKS(onDelay));

//         digitalWrite(LED_GPIO, LOW);
//         vTaskDelay(pdMS_TO_TICKS(offDelay));
//     }

//     // Nhỏ delay để tránh loop quá nhanh
//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }