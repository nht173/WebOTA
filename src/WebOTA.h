#ifndef WEB_OTA_H
#define WEB_OTA_H

#include <functional>
#include <Ticker.h>
#include <map>

 #if __has_include_next(<ESPAsyncWebServer.h>)
 #define USE_OTA_ASYNC
 #endif

/* Server type */
#ifdef USE_OTA_ASYNC
#include <ESPAsyncWebServer.h>
#define OTAWS_TYPE AsyncWebServer
#else
#if defined(ESP32)
#include <WebServer.h>
#define OTAWS_TYPE WebServer
#elif defined(ESP8266)
#include <ESP8266WebServer.h>
#define OTAWS_TYPE ESP8266WebServer
#endif
#endif

#ifndef UPDATE_SIZE_UNKNOWN
#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
#endif


class WebOTA {
public:
    WebOTA() = default;
    
    /**
     * @brief Construct a new Web OTA object
     * 
     * @param server web server instance
     * @param endpoint custom endpoint for OTA update. Default is "/update"
     */
    explicit WebOTA(OTAWS_TYPE& server, const String& endpoint = "/update") {
        _server = &server;
        _path = endpoint;
    }
    
    /**
     * @brief Start OTA update server
     * 
     * @param server 
     */
    void begin(OTAWS_TYPE& server) {
        _server = &server;
        begin();
    }
    
    /**
     * @brief Start OTA update server
     * 
     */
    void begin();


    /**
     * @brief Set the endpoint for OTA update
     * 
     * @param path 
     */
    void setEndpoint(const String& path) {
        _path = path;
    }

    /**
     * @brief Set the endpoint for OTA update
     * 
     * @param path 
     */
    void setEndpoint(const char* path) {
        _path = path;
    }

    /**
     * @brief Get the endpoint for OTA update
     *
     * @return String
     */
    String getEndpoint() const {
        return _path;
    }

    /**
     * @brief Set the device info for OTA update
     * 
     * @param deviceFWVersion 
     * @param deviceName 
     * @param deviceID 
     */
    void setDeviceInfo(const String& deviceFWVersion, const String& deviceName, const String& deviceID = "") {
        _parameters["FIRMWARE_VERSION"] = deviceFWVersion;
        _parameters["DEVICE_NAME"] = deviceName;
        _parameters["DEVICE_ID"] = deviceID;
    }

    /**
     * @brief Set the parameter for the OTA web page
     *
     * @param name
     * @param value
     */
    void setParameter(const String& name, const String& value) {
        _parameters[name] = value;
    }

    /**
     * @brief Get the device info for OTA update
     *
     * @param name
     * @return String
     */
    String getParameter(const String& name) {
        if (_parameters.find(name) != _parameters.end()) {
            return _parameters[name];
        }
        return "";
    }


    /**
     * @brief Set custom HTML page for OTA update.
     * 
     * @param html 
     */
    void setUpdatePage(const String& html) {
        if (!html.length()) return;
        static String staticHtml;
        staticHtml = html;
        _customUpdatePage = staticHtml.c_str();
        _isPageHtml = true;
    }


    /**
     * @brief Set custom HTML page for OTA update.
     * 
     * @param html 
     */
    void setUpdatePage(const char* html) {
        if (!html) return;
        _customUpdatePage = html;
        _isPageHtml = true;
    }


    /**
     * @brief Set custom gzip page for OTA update.
     *
     * @param gzip
     * @param len
     */
    void setUpdatePage(const uint8_t* gzip, size_t len) {
        _customUpdatePageBuffer = gzip;
        _customUpdatePageBufferLen = len;
    }


    /**
     * @brief Get JSON string of parameters
     * @return
     */
    String getParametersJSON() {
        String json = "{";
        for (const auto& param : _parameters) {
            json += "\"" + param.first + "\": \"" + param.second + "\",";
        }
        if (json.length() > 1) {
            json.remove(json.length() - 1); // remove last comma
        }
        json += "}";
        return json;
    }

    

    // Callback OTA events
    void onStart(std::function<void(const String& filename)> cb) {
        _onStart = cb;
    }
    
    void onProgress(std::function<void(size_t, size_t)> cb) {
        _onProgress = cb;
    }

    void onEnd(std::function<void()> cb) {
        _onEnd = cb;
    }

    void onError(std::function<void(int err)> cb) {
        _onError = cb;
    }

private:
    String _path = "/update";
    Ticker _ticker;
    OTAWS_TYPE* _server{};
    bool _isPageHtml = false;
    const char* _customUpdatePage = nullptr;
    const uint8_t* _customUpdatePageBuffer = nullptr;
    size_t _customUpdatePageBufferLen = 0;
    std::map<String, String> _parameters;

    // OTA callbacks
    std::function<void(const String&)> _onStart;
    std::function<void(size_t, size_t)> _onProgress;
    std::function<void()> _onEnd;
    std::function<void(int)> _onError;
};

#endif // WEB_OTA_H