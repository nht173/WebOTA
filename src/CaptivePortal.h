#ifndef CAPTIVE_PORTAL_H
#define CAPTIVE_PORTAL_H

#include "WebOTAType.h"
#include <DNSServer.h>

#if __has_include_next(<ESPAsyncWebServer.h>)
#define USE_CAPTIVE_PORTAL_ASYNC
#endif

class CaptivePortal {
public:
    friend class WiFiConfig;

    CaptivePortal() = default;

    explicit CaptivePortal(OTAWS_TYPE& server)
        : _server(&server) {};

    explicit CaptivePortal(OTAWS_TYPE* server, DNSServer* dnsServer)
        : _server(server), _dnsServer(dnsServer) {};
    
    ~CaptivePortal() {
        stop();
    }

    void setServer(OTAWS_TYPE* server) {
        _server = server;
    }

    OTAWS_TYPE* getServer() {
        return _server;
    }
    
    void setDNSServer(DNSServer* dnsServer) {
        _dnsServer = dnsServer;
    }
    
    DNSServer* getDNSServer() {
        return _dnsServer;
    }

    void setUri(const String& uri) {
        _uri = uri;
    }

    String getUri() const {
        return _uri;
    }

    void begin(const char* ssid, const char* password, const String& uri);
    
    void begin(const String& uri = "");

    void stop();

    void handleClient();

private:
    OTAWS_TYPE* _server{};
    DNSServer* _dnsServer{};
    String _uri = "/";
};

#endif // CAPTIVE_PORTAL_H