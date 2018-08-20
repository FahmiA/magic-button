# Magic Button

An IoT button that displays state and talks over MQTT to make something happen. Built for NodeMCU / ESP8266.

This project controls music (play / pause) but can be easily modified to control something else.

## Requirements

 - platform.io cli
 - MQTT server

## Configure

1. Edit `data/homie/config.json` with your WiFi and MQTT details.
2. Upload the data to your device's [SPIFFS](http://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-file-system-spiffs) storage.

    ```bash
    platformio run --target buildfs
    platformio run --target uploadfs
    ```

## Build

1. Build with platform.io.
    ```bash
    platformio run
    ```
