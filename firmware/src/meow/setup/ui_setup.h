#pragma once
#include <Arduino.h>
#include "../driver/graphics/GraphicsDriver.h"
#include "./screen_id.h"

// -------------------------------- Підключи нижче заголовкові файли екранів
#include "screen/splash/SplashScreen.h"
#include "screen/home/HomeScreen.h"
#include "screen/torch/TorchScreen.h"
#include "screen/menu/MenuScreen.h"
#include "screen/games/GamesListScreen.h"
#include "screen/mp3/Mp3Screen.h"
// #include "screen/calc/CalcScreen.h"
#include "screen/reader/ReaderScreen.h"
#include "screen/files/FilesScreen.h"
#include "screen/preferences/PrefSelectScreen.h"
#include "screen/preferences/PrefBrightScreen.h"
#include "screen/preferences/PrefWatchScreen.h"
#include "screen/preferences/PrefFileServerScreen.h"
#include "screen/firmware/FirmwareScreen.h"
// Ігрові екрани
#include "screen/games/sokoban/SokobanScreen.h"

// -------------------------------- Стартова орієнтація екрану
#define DISPLAY_ROTATION 2

// -------------------------------- Додай перемикання екранів
#define SCREEN_CASES                                 \
    case ScreenID::ID_SCREEN_HOME:                   \
        screen = new HomeScreen(_display);           \
        break;                                       \
    case ScreenID::ID_SCREEN_TORCH:                  \
        screen = new TorchScreen(_display);          \
        break;                                       \
    case ScreenID::ID_SCREEN_MENU:                   \
        screen = new MenuScreen(_display);           \
        break;                                       \
    case ScreenID::ID_SCREEN_MP3:                    \
        screen = new Mp3Screen(_display);            \
        break;                                       \
    case ScreenID::ID_SCREEN_READER:                 \
        screen = new ReaderScreen(_display);         \
        break;                                       \
    case ScreenID::ID_SCREEN_GAMES:                  \
        screen = new GamesListScreen(_display);      \
        break;                                       \
    case ScreenID::ID_SCREEN_FILES:                  \
        screen = new FilesScreen(_display);          \
        break;                                       \
    case ScreenID::ID_SCREEN_PREF_SEL:               \
        screen = new PrefSelectScreen(_display);     \
        break;                                       \
    case ScreenID::ID_SCREEN_PREF_BRIGHT:            \
        screen = new PrefBrightScreen(_display);     \
        break;                                       \
    case ScreenID::ID_SCREEN_PREF_FILE_SERVER:       \
        screen = new PrefFileServerScreen(_display); \
        break;                                       \
    case ScreenID::ID_SCREEN_PREF_WATCH:             \
        screen = new PrefWatchScreen(_display);      \
        break;                                       \
    case ScreenID::ID_SCREEN_FIRMWARE:               \
        screen = new FirmwareScreen(_display);       \
        break;                                       \
    case ScreenID::ID_SCREEN_SOKOBAN:                \
        screen = new SokobanScreen(_display);        \
        break;

// case ScreenID::ID_SCREEN_CALC:\
                //     screen = new CalcScreen(_display);\
                //     break;\

// -------------------------------- Стартовий екран
#define START_SCREEN SplashScreen