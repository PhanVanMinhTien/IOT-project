#include "led_blinky.h"

enum Led_Blinky_State { LED_IDLE, LED_ON, LED_OFF };
Led_Blinky_State led_blinky_State = LED_IDLE;

int blinkCount = 0;
int blinkTotal = 0;

int onDelay = 0;
int offDelay = 0;

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);

   while(1){
      // 1) Kiểm tra có nhiệt độ mới hay không
      if (xSemaphoreTake(xTempSemaphore, 0) == pdTRUE) {
          float t = glob_temperature;

            if (t < 25) {
                blinkTotal = 2;
                onDelay  = 1000;
                offDelay = 1000;
            } else if (t < 30) {
                blinkTotal = 4;
                onDelay  = 500;
                offDelay = 500;
            } else {
                blinkTotal = 8;
                onDelay  = 200;
                offDelay = 200;
            }

            blinkCount = 0;
            led_blinky_State = LED_ON;
        }

        // 2) Máy trạng thái cho LED
        switch (led_blinky_State) {

            case LED_ON:
                digitalWrite(LED_GPIO, HIGH);
                vTaskDelay(onDelay);
                led_blinky_State = LED_OFF;
                break;

            case LED_OFF:
                digitalWrite(LED_GPIO, LOW);
                vTaskDelay(offDelay);

                blinkCount++;
                if (blinkCount >= blinkTotal) {
                    led_blinky_State = LED_IDLE;
                } else {
                    led_blinky_State = LED_ON;
                }
                break;

            case LED_IDLE:
                // LED idle → không blink
                digitalWrite(LED_GPIO, LOW);
                vTaskDelay(100);
                break;
        }
    }
}