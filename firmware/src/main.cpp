#include <Arduino.h>
#include "meow/manager/screen/ScreenManager.h"

void guiTask(void *params)
{
    meow::MEOW.run();
}

void setup()
{
    // Serial.begin(115200);

    #if (ESP_ARDUINO_VERSION_MAJOR < 3)
    #pragma GCC error "Для коректної роботи декількох девайсів на одній шині SPI, необхідно застосувати патч"
        // Для коректної роботи, необхідно застосувати цей патч. Якщо патч вже встановлено, закоментуй дерективу.
        // Посилання на сторінку з патчем https://github.com/espressif/arduino-esp32/commit/629ffc55ed97b561f5bd1412a40cc83d00b2f825
        // Файл до якого потрібно застосувати патч C:\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-spi.c
    #endif

    xTaskCreatePinnedToCore(guiTask, "guiTask", (1024 / 2) * 100, NULL, 10, NULL, 1);
}

void loop()
{
    vTaskDelete(NULL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}