# Changelog

## [1.1.0] - 2025-10-23

### Added

- Captive portal support via a new `CaptivePortal` utility: allows the device to run as a soft AP and redirect connected clients (via DNS) to the OTA web page.
- Examples demonstrating captive-portal OTA:
  - `examples/WebServer/CaptivePortalOTA.ino` (WebServer)
  - `examples/AsyncWebServer/CaptivePortalAsyncOTA.ino` (ESPAsyncWebServer)

## [1.0.2] - 2025-05-28

### Changed

- Replace Ticker with esp_timer for ESP32

## [1.0.1] - 2025-05-14

### Added

- Nyan cat animation on the web interface during firmware upload.

## [1.0.0] - 2025-05-11

### Added

- Initial release with basic project setup.
