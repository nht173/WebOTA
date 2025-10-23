/**
 * This example demonstrates how to use the WebOTA library together with
 * the CaptivePortal utility to provide OTA updates via a captive portal
 * (device runs as an access point and redirects clients to the OTA page).
 *
 * Usage:
 *  - Flash this sketch
 *  - Connect to WiFi SSID: AP_SSID (default "OTA-Portal")
 *  - Open any URL in a browser; the captive portal should redirect to /update
 */

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WebOTA.h>
#include <CaptivePortal.h>

// Use the OTAWS_TYPE from WebOTAType.h so the correct server type is used
// (AsyncWebServer or WebServer/ESP8266WebServer) depending on available libs.
OTAWS_TYPE server(80);

#define FIRMWARE_VERSION "1.0.0"
#define AP_SSID "OTA-Portal"
#define AP_PASSWORD "" // empty for open AP (change if you want WPA)

WebOTA ota(server);
CaptivePortal portal(server); // we'll attach server to it in setup

void setup() {
    Serial.begin(115200);
    delay(2000);

    // Set device info for OTA page
    ota.setDeviceInfo(String(FIRMWARE_VERSION), "ESP Captive OTA Device");

    // Optional callbacks to observe update progress
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

    // Start WebOTA (this will register the /update endpoint on the server)
    ota.begin();
    server.begin();

    Serial.println("Captive portal started");
    Serial.printf("Connect to SSID: %s\n", AP_SSID);
    Serial.printf("Or open http://%s/update\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
    // Let captive portal handle DNS and web clients. For non-async servers
    // CaptivePortal::handleClient will call server.handleClient().
    portal.handleClient();
}
