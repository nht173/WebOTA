#include "CaptivePortal.h"

void CaptivePortal::begin(const char* ssid, const char* password, const String& uri) {
    WiFi.mode(WIFI_AP);
    if (ssid) {
        WiFi.softAP(ssid, password);
    }
    begin(uri);
}

void CaptivePortal::begin(const String& uri) {
    if (uri.length()) _uri = uri;
    if (!_server) _server = new OTAWS_TYPE(80);
    if (!_dnsServer) _dnsServer = new DNSServer();

    _dnsServer->setTTL(300);
    _dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    _dnsServer->start(53, "*", WiFi.softAPIP());

#if defined(USE_CAPTIVE_PORTAL_ASYNC)
    _server->on("/generate_204", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", _uri);
        request->send(response);
    });
  
    // microsoft redirect
    _server->on("/redirect", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", _uri);
        request->send(response);
    });
    
    // apple call home
    _server->on("/hotspot-detect.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", _uri);
        request->send(response);
    });

    // firefox captive portal call home
    _server->on("/canonical.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", _uri);
        request->send(response);
    });

    // firefox captive portal call home
    _server->on("/success.txt", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "success");
    });
    
    // windows call home
    _server->on("/ncsi.txt", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", _uri);
        request->send(response);
    });
#else
    _server->on("/generate_204", [this]() {
        _server->sendHeader("Location", _uri, true);
        _server->send(302, "text/plain", "");
    });

    // microsoft redirect
    _server->on("/redirect", [this]() {
        _server->sendHeader("Location", _uri, true);
        _server->send(302, "text/plain", "");
    });

    // apple call home
    _server->on("/hotspot-detect.html", [this]() {
        _server->sendHeader("Location", _uri, true);
        _server->send(302, "text/plain", "");
    });

	// firefox captive portal call home
    _server->on("/canonical.html", [this]() {
        _server->sendHeader("Location", _uri, true);
        _server->send(302, "text/plain", "");
    });

	// firefox captive portal call home
    _server->on("/success.txt", [this]() {
        _server->send(200, "text/plain", "success");
    });
	
    // windows call home
    _server->on("/ncsi.txt", [this]() {
        _server->sendHeader("Location", _uri, true);
        _server->send(302, "text/plain", "");
    });
#endif // USE_CAPTIVE_PORTAL_ASYNC
  
	
    _server->begin();
}


void CaptivePortal::stop() {
    if (_server) {
#if defined(USE_CAPTIVE_PORTAL_ASYNC)
        _server->end();
#else
        _server->stop();
#endif
        delete _server;
        _server = nullptr;
    }
    if (_dnsServer) {
        _dnsServer->stop();
        delete _dnsServer;
        _dnsServer = nullptr;
    }
}


void CaptivePortal::handleClient() {
    if (_dnsServer) _dnsServer->processNextRequest();
#ifndef USE_CAPTIVE_PORTAL_ASYNC
    if (_server) _server->handleClient();
#endif
}