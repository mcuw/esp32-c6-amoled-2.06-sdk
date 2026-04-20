# ESP32-C6 AMOLED 2.06" library

## Description

This is a library for the ESP32-C6 AMOLED 2.06" Watch from [waveshare](https://www.waveshare.com).

https://www.waveshare.com/product/arduino/boards-kits/esp32-c6/esp32-c6-touch-amoled-2.06.htm

## Prerequisites

- Uses Platformio/ pioarduino

## Features 

Quick kickstart a Watch project with a SDK


## Roadmap

System
- [x] ESP32-C6 RISC-V 32-bit processor, up to 160 MHz frequency, 512KB HP SRAM, 16KB LP SRAM, 320KB ROM, and 16MB Flash memory
- [x] Serialport
- [x] GFX support
- [x] Button - click, double click, long pressed
- [x] RTOS task
- [x] OTA upload over WiFi AP
- [x] OTA update with GitHub w/o cable and device is in production
- [ ] LVGL support for AMOLED 2.06", 410x502px resolution, 600 cd/㎡, CO5300 Display Driver
- [ ] AXP2101 PMU power management
- [ ] FT3168 capacitive touch chip
- [ ] preferences/ NVS

Energy management
- [ ] WiFi 6 iTWT - omnipresent
- [ ] BT 5 LE - connect to smartphone
- [ ] low energy consumption

Sensor and actors
- [ ] Watch - RTC PCF85063 chip sync with NTP
- [ ] Audio Playback
- [ ] Dual MIC Array Audio Recording
- [ ] QMI8658 6-Axis IMU (3-axis accelerometer and 3-axis gyroscope)

Connectivity
- [ ] Thread/ Matter with IEEE 802.15.4 wireless infrastructureless communication
- [ ] EspNow
- [ ] Webserver example: WiFi Manager 2 with Captive portal, Astro, Qwik

Release SDK
- [ ] publish to Arduino registry

## Hardware Description

- ESP32-C6 AMOLED 2.06" capacitive touch 410×502px display from waveshare

## Get Started

Uncomment an example in `platform.ini` then flash your watch.

## Supported boards

If you want to support this project, you can use these affiliate links ...

- ESP32 C6 with WiFi 6 and BT-5 LE
  - [ESP32-C6 AMOLED 2.06inch Touch Watch (aliexpress affiliate link)](https://s.click.aliexpress.com/e/_c4madtnD) with 16MB flash

## Github action / CI

- ensure that "Read and write permissions" option is activated under the Settings/ General/ Actions/ General - Workflow permissions (required for OTA update)

## Disclaimer

Contribution and help - if you find an issue or wants to contribute then please do not hesitate to create a pull request or an issue.

We provide our build template as is, and we make no promises or guarantees about this code.
