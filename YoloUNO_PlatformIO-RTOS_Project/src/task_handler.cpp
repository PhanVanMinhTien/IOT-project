#include <task_handler.h>
#include <ArduinoJson.h>
#include "led_blinky.h"      
#include "task_handler.h"


static const int BUILTIN_LED_CHANNEL = 0;

void handleWebSocketMessage(String message)
{
    Serial.println(message);
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
        Serial.println("❌ Error parse JSON!");
        return;
    }

    JsonObject value = doc["value"];
    String page = doc["page"].as<String>();

    // ==================== ĐIỀU KHIỂN GPIO (page = "device") ====================
    if (page == "device")
    {
        if (!value.containsKey("gpio") || !value.containsKey("status"))
        {
            Serial.println("⚠️ JSON thiếu thông tin gpio hoặc status");
            return;
        }

        int gpio = value["gpio"];
        String status = value["status"].as<String>();

        Serial.printf("⚙️ Điều khiển GPIO %d → %s\n", gpio, status.c_str());
        pinMode(gpio, OUTPUT);
        if (status.equalsIgnoreCase("ON"))
        {
            digitalWrite(gpio, HIGH);
            Serial.printf("🔆 GPIO %d ON\n", gpio);
        }
        else if (status.equalsIgnoreCase("OFF"))
        {
            digitalWrite(gpio, LOW);
            Serial.printf("💤 GPIO %d OFF\n", gpio);
        }
    }
    // ==================== LƯU CẤU HÌNH (page = "setting") ====================
    else if (page == "setting")
    {
        String WIFI_SSID       = value["ssid"].as<String>();
        String WIFI_PASS       = value["password"].as<String>();
        String CORE_IOT_TOKEN  = value["token"].as<String>();
        String CORE_IOT_SERVER = value["server"].as<String>();
        String CORE_IOT_PORT   = value["port"].as<String>();

        Serial.println("📥 Nhận cấu hình từ WebSocket:");
        Serial.println("SSID: " + WIFI_SSID);
        Serial.println("PASS: " + WIFI_PASS);
        Serial.println("TOKEN: " + CORE_IOT_TOKEN);
        Serial.println("SERVER: " + CORE_IOT_SERVER);
        Serial.println("PORT: " + CORE_IOT_PORT);

        // 👉 Gọi hàm lưu cấu hình
        Save_info_File(WIFI_SSID, WIFI_PASS, CORE_IOT_TOKEN, CORE_IOT_SERVER, CORE_IOT_PORT);

        // Phản hồi lại client (tùy chọn)
        String msg = "{\"status\":\"ok\",\"page\":\"setting_saved\"}";
        ws.textAll(msg);
    }

    // ==================== ĐIỀU CHỈNH ĐỘ SÁNG (page = "home") ====================
    else if (page == "home")
    {
        String type = value["type"].as<String>();
        if (type != "brightness")
        {
            return;
        }

        String target = value["target"].as<String>();
        int level     = value["level"] | 0;
        level = constrain(level, 0, 255);

        if (target == "builtin")
        {
            if (qBuiltinBrightness != nullptr)
            {
                xQueueOverwrite(qBuiltinBrightness, &level);
            }
        }
        else if (target == "neo")
        {
            if (qNeoBrightness != nullptr)
            {
                xQueueOverwrite(qNeoBrightness, &level);
            }
        }
        else
        {
            //
        }
    }
}
