#pragma GCC optimize("Ofast")
#include "ScreenManager.h"
#include "./screen/IScreen.h"
#include "../setup/screen_id.h"
#include "../setup/ui_setup.h"

namespace meow
{
    ScreenManager MEOW;

    void ScreenManager::run()
    {
        _display.setRotation(DISPLAY_ROTATION);
        _display.init();

        IScreen *screen = new START_SCREEN(_display);
        screen->show();

        // Основний цикл GUI.
        while (1)
        {
            if (!screen->isReleased())
                screen->tick();
            else
            {
                delete screen;
                switch (screen->getNextScreenID())
                {
                SCREEN_CASES
                default:
                    log_e("Некоректний screen_id: %i", screen->getNextScreenID());
                    esp_restart();
                }
                screen->show();
            }
        }
    }

}