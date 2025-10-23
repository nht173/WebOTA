#ifndef WEBOTA_TYPE_H
#define WEBOTA_TYPE_H

#if __has_include_next(<ESPAsyncWebServer.h>)
#include <ESPAsyncWebServer.h>
using OTAWS_TYPE = AsyncWebServer;
#else
#if defined(ESP32)
#include <WebServer.h>
using OTAWS_TYPE = WebServer;
#elif defined(ESP8266)
#include <ESP8266WebServer.h>
using OTAWS_TYPE = ESP8266WebServer;
#endif // defined(ESP32)
#endif // __has_include_next

#endif // WEBOTA_TYPE_H