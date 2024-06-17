#include <Arduino.h>
#include "meow/ui/ScreenManager.h"

void guiTask(void *params)
{
    // Serial.begin(115200);

    meow::MEOW.run();
}

void setup()
{
    xTaskCreatePinnedToCore(guiTask, "guiTask", (1024 / 2) * 100, NULL, 10, NULL, 1);
}

void loop()
{
    vTaskDelete(NULL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}