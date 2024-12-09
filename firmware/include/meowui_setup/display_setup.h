#pragma once
#include <Arduino.h>

// #include "User_Setups/Setup_lilka_v2_st7789.h" // Файл налаштування драйвера дисплею. 
#include <User_Setups/Setup304_ST7789_game_console.h>

#define BACKLIGHT_PIN (uint8_t)39  // Пін управління підсвіткою дисплея. Закоментуй якщо відсутнє управління підсвіткою
#define PWM_FREQ (uint16_t)3000 // Частота PWM підсвітки дисплея
#define PWM_CHANEL (uint8_t)0
#define PWM_RESOLUTION (uint8_t)8