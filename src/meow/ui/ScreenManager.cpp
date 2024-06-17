#include "ScreenManager.h"
#include "./screen/IScreen.h"
#include "./ScreenID.h"

// ------------------------------------------------------ Підключи тут екрани

#include "screen/splash/SplashScreen.h"
#include "screen/home/HomeScreen.h"
#include "screen/torch/TorchScreen.h"
#include "screen/menu/MenuScreen.h"
#include "screen/games/GamesListScreen.h"
#include "screen/mp3/Mp3Screen.h"
// #include "screen/calc/CalcScreen.h"
// #include "screen/reader/ReaderScreen.h"
#include "screen/files/FilesScreen.h"
#include "screen/preferences/PrefSelectScreen.h"
#include "screen/preferences/PrefBrightScreen.h"
#include "screen/preferences/PrefWatchScreen.h"
#include "screen/firmware/FirmwareScreen.h"
// Ігрові екрани
#include "screen/games/sokoban/SokobanScreen.h"

// ------------------------------------------------------

namespace meow
{
    ScreenManager MEOW;

#define DISPLAY_ROTATION 2 // --------------------------- Стартова орієнтація екрану

    void ScreenManager::run()
    {
        _display.setRotation(DISPLAY_ROTATION);
        _display.init();

        // -------------------------------------------------- Встанови стартовий екран
        IScreen *screen = new SplashScreen(_display);
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
                // --------------------------------------- Відредагуй switch під свої екрани
                case ScreenID::ID_SCREEN_HOME:
                    screen = new HomeScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_TORCH:
                    screen = new TorchScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_MENU:
                    screen = new MenuScreen(_display);
                    break;
                // case ScreenID::ID_SCREEN_MP3:
                //     screen = new Mp3Screen(_display);
                //     break;
                // case ScreenID::ID_SCREEN_CALC:
                //     screen = new CalcScreen(_display);
                //     break;
                // case ScreenID::ID_SCREEN_READER:
                //     screen = new ReaderScreen(_display);
                //     break;
                case ScreenID::ID_SCREEN_GAMES:
                    screen = new GamesListScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_FILES:
                    screen = new FilesScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_PREF_SEL:
                    screen = new PrefSelectScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_PREF_BRIGHT:
                    screen = new PrefBrightScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_PREF_WATCH:
                    screen = new PrefWatchScreen(_display);
                    break;
                case ScreenID::ID_SCREEN_FIRMWARE:
                    screen = new FirmwareScreen(_display);
                    break;
                    // --------------------------------------- Ігрові екрани
                case ScreenID::ID_SCREEN_SOKOBAN:
                    screen = new SokobanScreen(_display);
                    break;
                default:
                    log_e("Некоректний screen_id: %i", screen->getNextScreenID());
                    esp_restart();
                }
                screen->show();
            }
        }
    }

}