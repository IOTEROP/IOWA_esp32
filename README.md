![IOWA Logo](https://github.com/IOTEROP/IOWA/raw/master/.images/IoTerop_logo.jpg)

(WORK IN PROGRESS)

# IOWA samples for ESP32

This repository contains some IOWA samples ported to the ESP32 family. Two flavors...two folders !

## PlatfomIO

For the users of the famous [PlatformIO](https://platformio.org/), you can find a working example under the ./PlatformIO folder. You just have to open the project under VSCode PlatformIO.
Keep in mind that you to use *menuconfig* under the terminal with the following command:
`pio run -t menuconfig`
(Note: if you are not able to use the arrow to move through menuconfig, you can use K and J keys...it helps - PlatformIO issue)


## ESP_IDF framework

It is based on the ESP-IDF version 4.1 ([Espressif IoT Development Framework](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/))

Some of these samples can be build and tested with the [IOWA public code](https://github.com/IOTEROP/IOWA) [`https://github.com/IOTEROP/IOWA](https://github.com/IOTEROP/IOWA).
If exist, to build the _Advanced Samples_ (e.g: Firmware update), you need the *Full SDK* (please contact us at [https://ioterop.com/iowa/](https://ioterop.com/iowa/)).

Please note: **Neither the SDK nor the samples are open source software but commercial software.** and this code *should not* be used in a commercial product and is for personal evaluation ONLY !

| Category | Sample name | Purpose |
| --- | --- | --- |
| Basic samples | **ESP32_01_baseline_client** | For creating a working LwM2M Client with IOWA on ESP32 |
|  | **ESP32_02_LED** | Manage onboard LED with custom object |


1. Install, configure and test ESP-IDF with the _Blink_ sample (from esp-idf)
   Please refer to [https://docs.espressif.com/projects/esp-idf/en/stable/get-started/](https://docs.espressif.com/projects/esp-idf/en/stable/get-started) to install the framework according to your OS
2. Clone this repo with submodule:
`git clone --recurse-submodules https://github.com/IOTEROP/iowa_esp32.git`
3. Change to a sample directory (e.g.: `cd esp_samples/ESP32_01_baseline_client`)
4. Configure the options with the `idf.py menuconfig` command.
   - Configure connectivity with _Example Connection Configuration_ component (this component is provided by the framework). You have to configure the _WiFi SSID_ and the _WiFi Password_
   - Configure _IOWA sample config_ if needed (you can customize the Object name or the LwM2M server)
      The default name is _IOWA_Sample_client_ and the URI server points to our _CONNECTicut_ test server [CONNECTicut web page](https://iowa-server.ioterop.com)
5. Now you can build, flash and test your code with the usual command:
   `idf.py build flash monitor`

# Tutorials

You can find some interesting tutorials on [Hackster.io](https://www.hackster.io/)

- [Device management with LwM2M IOWA stack - Tutorial 1](https://www.hackster.io/ioterop/device-management-with-lwm2m-iowa-stack-tutorial-1-e5aaf8)

- [Device management with LwM2M IOWA stack #2: ESP32](https://www.hackster.io/ioterop/device-management-with-lwm2m-iowa-stack-2-esp32-fcb294)
