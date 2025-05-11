/**
 * This example demonstrates how to use the WebOTA library to perform OTA updates
 * on ESP32/ESP8266 devices using ESPAsyncWebServer library.
 * Library source: esp32async/ESPAsyncWebServer
 */

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <ESPAsyncWebServer.h>
#include <WebOTA.h>

#define FIRMWARE_VERSION "1.0.0"
#define SSID "YOUR_SSID"
#define PASSWORD "YOUR_PASSWORD"

AsyncWebServer server(80);
WebOTA ota(server);

void setup() {
    Serial.begin(115200);
    delay(2000); // wait for serial

    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to WiFi");

    // Callback event when WiFi is connected
    WiFi.onEvent([](WiFiEvent_t event) {
#ifdef ESP32
        if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            Serial.println("Connected to WiFi");
            Serial.printf("You can OTA at http://%s/update\n", WiFi.localIP().toString().c_str());
        }
#else
        if (event == WIFI_EVENT_STAMODE_GOT_IP) {
            Serial.println("Connected to WiFi");
            Serial.printf("You can OTA at http://%s/update\n", WiFi.localIP().toString().c_str());
        }
#endif
    });

    // Set device info for OTA update
    ota.setDeviceInfo(String(FIRMWARE_VERSION), "ESP Demo device");

    // Callbacks
    ota.onStart([](const String &filename) {
        Serial.printf("[OTA] Start updating %s\n", filename.c_str());
    });
    ota.onProgress([](size_t current, size_t total) {
        Serial.printf("[OTA] Progress: %u%%\n", (current / (total / 100)));
    });
    ota.onEnd([]() {
        Serial.println("[OTA] Update finished");
    });
    ota.onError([](int err) {
        Serial.printf("[OTA] Error: %d\n", err);
    });

    ota.begin();
    server.begin();
}

void loop() {

}