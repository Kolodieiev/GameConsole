#pragma once
#include <Arduino.h>

// -------------------------------- Підключи файл з налаштуваннями параметрів дисплею в файлі нижче (28 Рядок)
// #include <User_Setup_Select.h>   // Розкоментуй для швидкого доступу. Потім закоментуй знову

#define BACKLIGHT_PIN (uint8_t)39  // Пін управління підсвіткою дисплея. Закоментуй якщо відсутнє управління підсвіткою
#define PWM_FREQ (uint16_t)3000 // Частота PWM підсвітки дисплея
#define PWM_CHANEL (uint8_t)0
#define PWM_RESOLUTION (uint8_t)8