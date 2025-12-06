#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "temp_humi_monitor.h"
#include "lcd_task.h"
//#include "mainserver.h"
#include "tinyml.h"
#include "coreiot.h"

// include task
#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
//#include "task_core_iot.h"

void debugPrintIP()
{
    static wifi_mode_t lastMode = WIFI_MODE_NULL;
    static IPAddress lastStaIP(0, 0, 0, 0);
    static IPAddress lastApIP(0, 0, 0, 0);

    wifi_mode_t mode = WiFi.getMode();

    // Nếu mode WiFi thay đổi -> in ra
    if (mode != lastMode)
    {
        Serial.print("📡 WiFi mode đổi thành: ");
        if (mode == WIFI_OFF)          Serial.println("WIFI_OFF");
        else if (mode == WIFI_STA)     Serial.println("WIFI_STA");
        else if (mode == WIFI_AP)      Serial.println("WIFI_AP");
        else if (mode == WIFI_AP_STA)  Serial.println("WIFI_AP_STA");
        else                           Serial.println((int)mode);

        lastMode = mode;
    }

    // Nếu có STA (kết nối router)
    if (mode == WIFI_STA || mode == WIFI_AP_STA)
    {
        IPAddress staIP = WiFi.localIP();
        // Chỉ in khi IP hợp lệ và khác IP trước đó
        if (staIP.toString() != "0.0.0.0" && staIP != lastStaIP)
        {
            Serial.print("🌐 IP STA: ");
            Serial.println(staIP);
            lastStaIP = staIP;
        }
    }

    // Nếu có AP (phát WiFi)
    if (mode == WIFI_AP || mode == WIFI_AP_STA)
    {
        IPAddress apIP = WiFi.softAPIP();
        if (apIP.toString() != "0.0.0.0" && apIP != lastApIP)
        {
            Serial.print("📶 IP AP: ");
            Serial.println(apIP);
            lastApIP = apIP;
        }
    }
}



void setup()
{
  Serial.begin(115200);
  check_info_File(0);

  xTaskCreate(led_blinky, "Task LED Blink", 2048, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
  xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 2048, NULL, 2, NULL);
  xTaskCreate(lcd_task, "Task LCD Display", 2048, NULL, 2, NULL);
  //xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);
  xTaskCreate( tiny_ml_task, "Tiny ML Task" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate(coreiot_task, "CoreIOT Task" ,4096  ,NULL  ,2 , NULL);
  // xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);
}

// void loop()
// {
//   if (check_info_File(1))
//   {
//     if (!Wifi_reconnect())
//     {
//       Webserver_stop();
//     }
//     else
//     {
//       CORE_IOT_reconnect();
//     }
//   }
//   Webserver_reconnect();
// }

void loop()
{
  // 🧪 In IP mỗi khi có thay đổi mode / IP (AP hoặc STA)
  debugPrintIP();

  if (check_info_File(1))
  {
    if (!Wifi_reconnect())
    {
      Webserver_stop();
    }
    else
    {
      //CORE_IOT_reconnect();
    }
  }

  Webserver_reconnect();
}
