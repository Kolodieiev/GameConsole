#pragma once
#include <Arduino.h>

// В даному проєкті карта пам'яті одна на своїй шині. 
// Тому пін CS задано такий, що ніде не використовується на платі.
#define SD_PIN_CS 46

//ESP32S3. FSPI
#define SD_PIN_MISO 13 
#define SD_PIN_MOSI 11 
#define SD_PIN_SCLK 12 

//ESP32S3. HSPI. Уточнити!
// #define SD_PIN_MISO 40 
// #define SD_PIN_MOSI 42 
// #define SD_PIN_SCLK 41

//ESP32. VSPI
// #define SD_PIN_CS 5
// #define SD_PIN_MISO 19
// #define SD_PIN_MOSI 23
// #define SD_PIN_SCLK 18

//ESP32. HSPI
// #define SD_PIN_MISO 12
// #define SD_PIN_MOSI 13
// #define SD_PIN_SCLK 14


#define SD_FREQUENCY 80000000
#define SD_MOUNTPOINT "/sd"
#define SD_MAX_FILES (uint8_t)255
