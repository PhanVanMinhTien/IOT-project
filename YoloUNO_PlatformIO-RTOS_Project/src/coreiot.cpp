#include "coreiot.h"
#include "temp_humi_monitor.h"


// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// -------------------- RPC CALLBACK (giữ lại logic cũ) --------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[CoreIOT] Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  // copy payload sang buffer có null-terminator
  char message[256];
  if (length >= sizeof(message)) length = sizeof(message) - 1;
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.print("[CoreIOT] Payload: ");
  Serial.println(message);

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print("[CoreIOT] deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* method = doc["method"];
  if (!method) {
    Serial.println("[CoreIOT] No method in RPC payload");
    return;
  }

  if (strcmp(method, "setStateLED") == 0) {
    const char* params = doc["params"];
    if (!params) {
      Serial.println("[CoreIOT] No params in RPC payload");
      return;
    }

    if (strcmp(params, "ON") == 0) {
      Serial.println("[CoreIOT] Device turned ON (RPC).");
      // TODO: bật LED / thiết bị thực tế
    } else {
      Serial.println("[CoreIOT] Device turned OFF (RPC).");
      // TODO: tắt LED / thiết bị thực tế
    }
  } else {
    Serial.print("[CoreIOT] Unknown method: ");
    Serial.println(method);
  }
}

// -------------------- MQTT RECONNECT --------------------
static void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("[CoreIOT] Attempting MQTT connection to ");
    Serial.print(CORE_IOT_SERVER);
    Serial.print(":");
    Serial.print(CORE_IOT_PORT);
    Serial.print(" ... ");

    // username = token, password = NULL theo template CoreIOT
    if (client.connect("ESP32Client", CORE_IOT_TOKEN.c_str(), nullptr)) {
      Serial.println("connected!");
      client.subscribe("v1/devices/me/rpc/request/+");
      //client.subscribe("v1/devices/me/rpc/request/+");

      Serial.println("[CoreIOT] Subscribed v1/devices/me/rpc/request/+");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" -> retry in 5 seconds");
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

// -------------------- SETUP COREIOT (chạy trong task) --------------------
static void setup_coreiot() {
  // 1) Chờ WiFi đã có Internet
  Serial.println("[CoreIOT] Waiting for internet semaphore...");
  if (xBinarySemaphoreInternet != nullptr) {
    xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY);
  }

  Serial.println("[CoreIOT] WiFi connected, starting MQTT client.");

  // 2) Kiểm tra cấu hình CoreIOT đã được nhập hay chưa
  if (CORE_IOT_SERVER.isEmpty() || CORE_IOT_TOKEN.isEmpty() || CORE_IOT_PORT.isEmpty()) {
    Serial.println("[CoreIOT] ERROR: CORE_IOT_SERVER / TOKEN / PORT rỗng.");
    Serial.println("[CoreIOT] Hãy vào web config, nhập đầy đủ rồi reboot.");
    // Không xoá task ngay, để log liên tục cũng được
  }

  // 3) Cài đặt server và callback
  client.setServer(CORE_IOT_SERVER.c_str(), CORE_IOT_PORT.toInt());
  client.setCallback(callback);
}

// -------------------- COREIOT TASK --------------------
void coreiot_task(void *pvParameters) {
    setup_coreiot();                      // chờ WiFi, set MQTT server, callback...

    const TickType_t publishInterval = pdMS_TO_TICKS(10000); // 10s/lần

    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("[CoreIOT] WiFi lost, waiting...");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        if (!client.connected()) {
            mqttReconnect();
        }
        client.loop();

        // ==== LẤY GIÁ TRỊ TỪ QUEUE DHT ====
        DHT_Data dhtData;
        float t = 0;
        float h = 0;

        if (xQueueDHT != nullptr &&
            xQueuePeek(xQueueDHT, &dhtData, 0) == pdTRUE)
        {
            t = dhtData.temperature;
            h = dhtData.humidity;
        } else {
            Serial.println("[CoreIOT] ⚠️ Không đọc được dữ liệu từ xQueueDHT, dùng giá trị 0.");
        }

        //
        // Serial.print("[CoreIOT] t = ");
        // Serial.print(t);
        // Serial.print(", h = ");
        // Serial.println(h);

        // ==== PUBLISH LÊN CORE IOT ====
        StaticJsonDocument<128> doc;
        doc["temperature"] = t;
        doc["humidity"]    = h;

        char payload[128];
        size_t n = serializeJson(doc, payload, sizeof(payload));

        if (n > 0) {
            bool ok = client.publish("v1/devices/me/telemetry", payload);
            //bool ok = client.publish("esp/telemetry", payload);
            if (ok) {
                //Serial.print("[CoreIOT] Published telemetry: ");
                //Serial.println(payload);
            } else {
                Serial.println("[CoreIOT] ❌ Failed to publish telemetry");
            }
        } else {
            Serial.println("[CoreIOT] ❌ serializeJson() = 0");
        }

        vTaskDelay(publishInterval);
    }
}



