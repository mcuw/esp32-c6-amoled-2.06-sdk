#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define SDA 8
#define SCL 7

// PMU AXP2101
#define PMU_SDA SDA
#define PMU_SCL SCL
// Touch sensor FT3168
#define TOUCH_SDA SDA
#define TOUCH_SCL SCL
#define TOUCH_RESET 10
#define TOUCH_INT 15
// Acclerometer and Gyroscope sensor QMI8658
#define QMI_SDA SDA
#define QMI_SCL SCL
#define QMI_INT1 16
#define QMI_INT2 17
// RTC PCF85063     
#define RTC_SDA SDA
#define RTC_SCL SCL
// Speaker ES8311
#define SPK_SDA SDA
#define SPK_SCL SCL
#define SPK_MCLK 19
#define SPK_SCLK 20

// ES7210 AEC I2S MIC PA
#define MIC_I2S_ASDOUT 21
#define MIC_I2S_LRCK 22
#define MIC_I2S_DSDIN 23
#define MIC_PA_CTRL 6

#define BUTTON_PWR 18
#define BUTTON_BOOT 9
// #define BUTTON_ACTIVEHIGH 0
// #define BUTTON_PULLUP 1

// AMOLED 2.06" LCD pins
#define LCD_SCLK 0
#define LCD_SDIO0 1
#define LCD_SDIO1 2
#define LCD_SDIO2 3
#define LCD_SDIO3 4
#define LCD_CS 5
#define LCD_RESET 11
#define LCD_WIDTH 410
#define LCD_HEIGHT 502

#define XPOWERS_CHIP_AXP2101

#define SS 20
#define MOSI 26
#define MISO 21
#define SCK 25
#define SPICS0 SS 
#define SPID MOSI
#define SPIQ MISO
#define SPICLK SCK

// #define USB_TX 12
// #define USB_RX 13


// #define TX 1
// #define RX 3

// #define PMU_IRQ 35 // Battery ADC
// #define PMU_WIRE_PORT Wire

// #define HAS_DISPLAY //Optional, bring your own board, no LCD !!
// #define DISPLAY_MODEL_SSD_LIB       SSD1306Wire
// #define DISPLAY_MODEL               U8G2_SSD1306_128X64_NONAME_F_HW_I2C

// static const uint8_t A0 = 36;
// static const uint8_t A3 = 39;
// static const uint8_t A4 = 32;
// static const uint8_t A5 = 33;
// static const uint8_t A6 = 34;
// static const uint8_t A7 = 35;

// static const uint8_t A10 = 4;
// static const uint8_t A11 = 0;
// static const uint8_t A12 = 2;
// static const uint8_t A14 = 13;
// static const uint8_t A16 = 14;
// static const uint8_t A18 = 25;

// static const uint8_t T0 = 4;
// static const uint8_t T1 = 0;
// static const uint8_t T2 = 2;
// static const uint8_t T4 = 13;
// static const uint8_t T6 = 14;
// static const uint8_t T8 = 33;
// static const uint8_t T9 = 32;

// static const uint8_t DAC1 = 25;

#endif /* Pins_Arduino_h */
