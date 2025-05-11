#include "WebOTA.h"
#include "UPDATE_PAGE.h"

#if defined(ESP32)
#include <Update.h>
#elif defined(ESP8266)
#include <Updater.h>
#endif


void WebOTA::begin() {
    if (!_server) {
        Serial.println("WebOTA: Server not set. Call begin(WebServer& server) first.");
        return;
    }

    // Set default parameters
    if (getParameter("FIRMWARE_VERSION") == "")
        _parameters["FIRMWARE_VERSION"] = "Unknown version";
    if (getParameter("DEVICE_NAME") == "")
        _parameters["DEVICE_NAME"] = "ESP";
    if (getParameter("DEVICE_ID") == "") {
#if defined(ESP32)
        _parameters["DEVICE_ID"] = String(ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
        _parameters["DEVICE_ID"] = String(ESP.getChipId(), HEX);
#endif
    }

    /* api to return device info */
#if defined(USE_OTA_ASYNC)
    _server->on((_path + "/info").c_str(), HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "application/json", getParametersJSON());
    });
#else
    _server->on(_path + "/info", HTTP_GET, [this]() {
        _server->send(200, "application/json", getParametersJSON());
    });
#endif


    /* update page */
#if defined(USE_OTA_ASYNC)
    _server->on(_path.c_str(), HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (_isPageHtml) {
            request->send(200, "text/html", _customUpdatePage);
        }
        else if (_customUpdatePageBuffer && _customUpdatePageBufferLen) {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", _customUpdatePageBuffer, _customUpdatePageBufferLen);
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            // default page
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", UPDATE_PAGE, UPDATE_PAGE_len);
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        }
    });

    _server->on(_path.c_str(), HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (Update.hasError()) {
            request->send(400, "text/plain", "FAIL");
        } else {
            request->send(200, "text/plain", "OK");
        }
        _ticker.once(2, []() {
            ESP.restart();
        });
    }, [this](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len,
              bool final) {
        if (index == 0) {
            Serial.printf("Update Start: %s\n", filename.c_str());
            if (_onStart) _onStart(filename);
#if defined(ESP32)
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
#elif defined(ESP8266)
            Update.runAsync(true);
            uint32_t update_size = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if (!Update.begin(update_size, U_FLASH)) {
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
#endif
        }
        if (len) {
            if (Update.write(data, len) != len) {
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
            if (_onProgress) _onProgress(Update.progress(), Update.size());
        }
        if (final) {
            if (Update.end(true)) {
                Serial.printf("Update Success: %u bytes\nRebooting...\n", index + len);
                if (_onEnd) _onEnd();
            } else {
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
        }
    });

#else
    _server->on(_path, HTTP_GET, [this]() {
        if (_isPageHtml) {
            _server->send(200, "text/html", _customUpdatePage);
        }
        else if (_customUpdatePageBuffer && _customUpdatePageBufferLen) {
            _server->sendHeader("Content-Encoding", "gzip");
            _server->send_P(200, "text/html", reinterpret_cast<const char *>(_customUpdatePageBuffer), _customUpdatePageBufferLen);
        } else {
            // default page
            _server->sendHeader("Content-Encoding", "gzip");
            _server->send_P(200, "text/html", reinterpret_cast<const char *>(UPDATE_PAGE), UPDATE_PAGE_len);
        }
    });
    
    _server->on(_path, HTTP_POST, [this]() {
        if (Update.hasError()) {
            _server->send(400, "text/plain", "FAIL");
        } else {
            _server->send(200, "text/plain", "OK");
        }
        _ticker.once(2, []() {
            ESP.restart();
        });
    }, [this]() {
        HTTPUpload& upload = _server->upload();
        if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("Update Start: %s\n", upload.filename.c_str());
            if (_onStart) _onStart(upload.filename);
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
            if (_onProgress) _onProgress(Update.progress(), Update.size());
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) {
                Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize);
                if (_onEnd) _onEnd();
            } else {
                Update.printError(Serial);
                if (_onError) _onError(Update.getError());
            }
        } else if (upload.status == UPLOAD_FILE_ABORTED) {
            Update.printError(Serial);
            if (_onError) _onError(Update.getError());
        }
    });
#endif
}