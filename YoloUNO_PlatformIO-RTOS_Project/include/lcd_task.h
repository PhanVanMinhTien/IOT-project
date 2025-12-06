#ifndef LCD_TASK_H
#define LCD_TASK_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "semphr.h"

// // Struct dùng để chia sẻ dữ liệu giữa Sensor Task và LCD Task
// typedef struct {
//     float temperature;
//     float humidity;
//     int state_code;               // 0 = normal, 1 = warning, 2 = critical
//     SemaphoreHandle_t sem;        // semaphore báo có dữ liệu mới
// } SharedData_t;

// Prototype của LCD Task
void lcd_task(void *pvParameters);

#endif
