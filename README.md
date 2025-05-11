# ESP Web OTA

A project for ESP32 and ESP8266 microcontrollers using PlatformIO, providing asynchronous HTTP server capabilities with OTA (Over-The-Air) update support.

## Features

- Asynchronous HTTP and non-asynchronous web server
- OTA firmware updates via web interface
- Compatible with ESP32 and ESP8266 boards

## Installation

Add dependencies to your `platformio.ini` file:

```ini
lib_deps = 
    https://github.com/nht173/WebOTA.git^@1.0.0
```

## Usage

See the [example code](./examples) for a basic implementation of the ESP Web OTA functionality.

# Advanced Usage

## Set the device info to be displayed

You can set the device information to be displayed on the web page by modifying the `setDeviceInfo` method in the `WebOTA` class. By default, it is set to display the device name and IP address. You can change it to any other information you prefer.

```cpp
#define FIRMWARE_VERSION "1.0.0"
#define DEVICE_NAME "ESP32 Device"

webOTA.setDeviceInfo(FIRMWARE_VERSION, DEVICE_NAME);
```

## Callback Functions

You can define custom callback functions for various events during the OTA process. The following events are available:

- `onStart`: Called when the OTA process starts. Called before the firmware started downloading.
- `onEnd`: Called when the OTA process ends.
- `onProgress`: Called during the OTA process to report progress.
- `onError`: Called when an error occurs during the OTA process.

## Change endpoint

You can change the endpoint for the OTA process by modifying the `setEndpoint` method in the `WebOTA` class. By default, it is set to `/update`. You can change it to any other endpoint you prefer.

```cpp
webOTA.setEndpoint("/custom_update");
```

## Set custom HTML page

You can set a custom HTML page or gzip file for the OTA process by modifying the `setUpdatePage` method in the `WebOTA` class. By default, it is set to display a simple form for firmware upload. You can change it to any other HTML page you prefer.

## Set custom parameters

You can set custom parameters for the update web page by modifying the `setParameter` method in the `WebOTA` class.

You can get the parameters as JSON by GET request to `endpoint/info`

By default, it has 3 parameters: FIRMWARE_VERSION, DEVICE_NAME, and DEVICE_ID. You can change them to any other parameters you prefer.

---

# Future Improvements

- Add support authentication for OTA updates
- Add support for checking firmware version before updating****
