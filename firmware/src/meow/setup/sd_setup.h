#pragma once
#include <Arduino.h>

// В даному проєкті карта пам'яті одна на своїй шині.
// Тому пін CS задано такий, що ніде не використовується на платі.
#define SD_PIN_CS (int8_t)46

// ESP32S3
#define SD_SPI_BUS FSPI
#define SD_PIN_MISO (int8_t)13
#define SD_PIN_MOSI (int8_t)11
#define SD_PIN_SCLK (int8_t)12

// ESP32S3. Уточнити!
//  #define SD_SPI_BUS HSPI
//  #define SD_PIN_MISO (int8_t)40
//  #define SD_PIN_MOSI (int8_t)42
//  #define SD_PIN_SCLK (int8_t)41

// ESP32
//  #define SD_SPI_BUS VSPI
//  #define SD_PIN_CS (int8_t)5
//  #define SD_PIN_MISO (int8_t)19
//  #define SD_PIN_MOSI (int8_t)23
//  #define SD_PIN_SCLK (int8_t)18

// ESP32
//  #define SD_SPI_BUS HSPI
//  #define SD_PIN_MISO (int8_t)12
//  #define SD_PIN_MOSI (int8_t)13
//  #define SD_PIN_SCLK (int8_t)14

#define SD_FREQUENCY 80000000
#define SD_MOUNTPOINT "/sd"
#define SD_MAX_FILES (uint8_t)255
