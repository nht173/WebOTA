/**
 * This example demonstrates how to use the WebOTA library together with
 * the CaptivePortal utility to provide OTA updates via a captive portal
 * using ESPAsyncWebServer (async).
 *
 * Usage:
 *  - Flash this sketch
 *  - Connect to WiFi SSID: AP_SSID (default "OTA-Portal-Async")
 *  - Open any URL in a browser; the captive portal should redirect to /update
 */

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <ESPAsyncWebServer.h>
#include <WebOTA.h>
#include <CaptivePortal.h>

#define FIRMWARE_VERSION "1.0.0"
#define AP_SSID "OTA-Portal-Async"
#define AP_PASSWORD ""

AsyncWebServer server(80);
WebOTA ota(server);
CaptivePortal portal(server);

void setup() {
    Serial.begin(115200);
    delay(2000);

    // Set device info for OTA page
    ota.setDeviceInfo(String(FIRMWARE_VERSION), "ESP Async Captive OTA Device");

    // Callbacks
    ota.onStart([](const String &filename) {
        Serial.printf("[OTA] Start updating %s\n", filename.c_str());
    });
    ota.onProgress([](size_t current, size_t total) {
        if (total == 0) return;
        Serial.printf("[OTA] Progress: %u%%\n", (unsigned)((current * 100) / total));
    });
    ota.onEnd([]() {
        Serial.println("[OTA] Update finished");
    });
    ota.onError([](int err) {
        Serial.printf("[OTA] Error: %d\n", err);
    });

    // Start captive portal in AP mode. Clients connecting to the AP will be
    // redirected to the OTA page ("/update").
    portal.begin(AP_SSID, AP_PASSWORD, "/update");

    // Start WebOTA and server
    ota.begin();
    server.begin();

    Serial.println("Async captive portal started");
    Serial.printf("Connect to SSID: %s\n", AP_SSID);
    Serial.printf("Or open http://%s/update\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
    // For async server, CaptivePortal::handleClient() is no-op for request handling
    // but DNS needs processing
    portal.handleClient();
}
