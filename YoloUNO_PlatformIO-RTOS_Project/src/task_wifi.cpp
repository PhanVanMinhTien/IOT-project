#include "task_wifi.h"

void startAP()
{
    WiFi.disconnect(true, true);  
    WiFi.mode(WIFI_AP);   // hoặc WIFI_AP_STA nếu muốn giữ STA luôn
    WiFi.softAP(String(SSID_AP), String(PASS_AP));

    Serial.println("📡 Chuyển sang AP mode để cấu hình:");
    Serial.print("    SSID: "); Serial.println(SSID_AP);
    Serial.print("    PASS: "); Serial.println(PASS_AP);
    Serial.print("    AP IP: "); Serial.println(WiFi.softAPIP());
}



void startSTA()
{
    if (WIFI_SSID.isEmpty())
    {
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);

    if (WIFI_PASS.isEmpty())
    {
        WiFi.begin(WIFI_SSID.c_str());
    }
    else
    {
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    //Give a semaphore here
    xSemaphoreGive(xBinarySemaphoreInternet);
    Serial.println("\n✅ Kết nối WiFi thành công!");
    Serial.print("🌐 IP STA: ");
    Serial.println(WiFi.localIP());
}





bool Wifi_reconnect()
{
    static wl_status_t lastStatus = WL_DISCONNECTED;
    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED)
    {
        // Chỉ in khi vừa chuyển từ mất mạng -> có mạng
        if (lastStatus != WL_CONNECTED)
        {
            Serial.println("✅ WiFi đã kết nối.");
            Serial.print("🌐 IP STA: ");
            Serial.println(WiFi.localIP());
        }
        lastStatus = status;
        return true;
    }

    // Nếu trước đó là CONNECTED mà giờ mất thì báo 1 lần
    if (lastStatus == WL_CONNECTED)
    {
        Serial.println("⚠️ Mất WiFi, thử reconnect...");
    }

    lastStatus = status;

    // Thử kết nối lại
    startSTA();   // hàm này cũng in IP khi kết nối xong

    // Cập nhật lại trạng thái sau startSTA()
    lastStatus = WiFi.status();
    return (lastStatus == WL_CONNECTED);
}