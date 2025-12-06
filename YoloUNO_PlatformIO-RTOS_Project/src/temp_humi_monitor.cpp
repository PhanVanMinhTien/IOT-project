#include "temp_humi_monitor.h"
#include "task_webserver.h"


DHT20 dht20;



void temp_humi_monitor(void *pvParameters){ 
    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();
    
    while (1){
        /* code */
        dht20.read();
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();
        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        // Serial.print("Humidity: ");
        // Serial.print(humidity);
        // Serial.print("%  Temperature: ");
        // Serial.print(temperature);
        // Serial.println("°C");


        DHT_Data dht20_data;
        dht20_data.temperature = temperature;
        dht20_data.humidity = humidity; 
        if (xQueueDHT != NULL) {
            xQueueOverwrite(xQueueDHT, &dht20_data);
        }
        // Báo cho Task LED biết để cập nhật trạng thái đèn (Kể cả khi nhiệt độ bình thường)
        if (sem_led != NULL) xSemaphoreGive(sem_led);
            
        // Báo cho Task NeoPixel biết để cập nhật màu sắc
        if (sem_neo != NULL) xSemaphoreGive(sem_neo);


        Webserver_sendSensor(temperature, humidity);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    
}