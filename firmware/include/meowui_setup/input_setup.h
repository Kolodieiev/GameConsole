#pragma once
#include <Arduino.h>

// -------------------------------- Увімкнути підтримку сенсорного екрану
// #define TOUCHSCREEN_SUPPORT

#ifdef TOUCHSCREEN_SUPPORT
#define TOUCH_ROTATION ROTATION_0
#define TOUCH_WIDTH 320
#define TOUCH_HEIGHT 480

// -------------------------------- Обрати драйвер сенсорного екрану
#define GT911_DRIVER // Поки що додано підтримку тільки 1 чіпа


// -------------------------------- Налаштуй піни, до яких підключено драйвер тачскріна
#ifdef GT911_DRIVER
#define TOUCH_SDA_PIN 33
#define TOUCH_SCL_PIN 32
#define TOUCH_INT_PIN 21
#define TOUCH_RST_PIN 25
#endif// GT911_DRIVER

#endif // TOUCHSCREEN_SUPPORT

// -------------------------------- Закріплення назв кнопок до виводів МК

namespace meow
{
    enum KeyID : uint8_t
    {
        KEY_UP = 5,
        KEY_DOWN = 6,
        KEY_LEFT = 4,
        KEY_RIGHT = 7,
        KEY_OK = 8,
        KEY_BACK = 9
    };
}

// -------------------------------- Макрос з ініціалізацією пінів. Між фігурними дужками потрібно додати ініціалізацію пінів, як показано нижче
// -------------------------------- Де Pin(uint8_t key_id, bool is_touch)
#define BUTTONS                                \
    {                                          \
        {KEY_UP, new Pin(KEY_UP, true)},       \
        {KEY_DOWN, new Pin(KEY_DOWN, true)},   \
        {KEY_LEFT, new Pin(KEY_LEFT, true)},   \
        {KEY_RIGHT, new Pin(KEY_RIGHT, true)}, \
        {KEY_OK, new Pin(KEY_OK, true)},       \
        {KEY_BACK, new Pin(KEY_BACK, true)},   \
    }

// -------------------------------- Налаштування чутливості сенсорних кнопок
#if defined(CONFIG_IDF_TARGET_ESP32S3)
#define KEY_TOUCH_TRESHOLD 50000
#else
#define KEY_TOUCH_TRESHOLD 65
#endif

#define PRESS_DURATION (unsigned long)1000
//
#define PRESS_LOCK (unsigned long)700
#define CLICK_LOCK (unsigned long)250
#define HOLD_LOCK (unsigned long)150