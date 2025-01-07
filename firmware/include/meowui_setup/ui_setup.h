#pragma once
#include <Arduino.h>
#include "meow/driver/graphics/GraphicsDriver.h"
#include "./context_id.h"

// -------------------------------- Підключи нижче заголовкові файли екранів
#include "context/splash/SplashContext.h"
#include "context/home/HomeContext.h"
#include "context/torch/TorchContext.h"
#include "context/menu/MenuContext.h"
#include "context/games/GamesListContext.h"
#include "context/mp3/Mp3Context.h"
// #include "context/calc/CalcContext.h"
#include "context/reader/ReaderContext.h"
#include "context/files/FilesContext.h"
#include "context/preferences/PrefSelectContext.h"
#include "context/preferences/PrefBrightContext.h"
#include "context/preferences/PrefWatchContext.h"
#include "context/preferences/PrefFileServerContext.h"
#include "context/firmware/FirmwareContext.h"
// Ігрові екрани
#include "context/games/sokoban/SokobanContext.h"
#include "context/games/test_server/TestServerScreen.h"

// -------------------------------- Стартова орієнтація екрану
#define DISPLAY_ROTATION 2

// -------------------------------- Додай перемикання екранів
#define SCREEN_CASES                                          \
    case ContextID::ID_CONTEXT_HOME:                            \
        context = new HomeContext(_display);                    \
        break;                                                \
    case ContextID::ID_CONTEXT_TORCH:                           \
        context = new TorchContext(_display);                   \
        break;                                                \
    case ContextID::ID_CONTEXT_MENU:                            \
        context = new MenuContext(_display);                    \
        break;                                                \
    case ContextID::ID_CONTEXT_MP3:                             \
        context = new Mp3Context(_display);                     \
        break;                                                \
    case ContextID::ID_CONTEXT_READER:                          \
        context = new ReaderContext(_display);                  \
        break;                                                \
    case ContextID::ID_CONTEXT_GAMES:                           \
        context = new GamesListContext(_display);               \
        break;                                                \
    case ContextID::ID_CONTEXT_FILES:                           \
        context = new FilesContext(_display);                   \
        break;                                                \
    case ContextID::ID_CONTEXT_PREF_SEL:                        \
        context = new PrefSelectContext(_display);              \
        break;                                                \
    case ContextID::ID_CONTEXT_PREF_BRIGHT:                     \
        context = new PrefBrightContext(_display);              \
        break;                                                \
    case ContextID::ID_CONTEXT_PREF_FILE_SERVER:                \
        context = new PrefFileServerContext(_display);          \
        break;                                                \
    case ContextID::ID_CONTEXT_PREF_WATCH:                      \
        context = new PrefWatchContext(_display);               \
        break;                                                \
    case ContextID::ID_CONTEXT_FIRMWARE:                        \
        context = new FirmwareContext(_display);                \
        break;                                                \
    case ContextID::ID_CONTEXT_SOKOBAN:                         \
        context = new sokoban::SokobanContext(_display);        \
        break;                                                \
    case ContextID::ID_CONTEXT_TEST_SERVER:                     \
        context = new test_server::TestServerScreen(_display); \
        break;

// case ContextID::ID_CONTEXT_CALC:\
                //     context = new CalcContext(_display);\
                //     break;\

// -------------------------------- Стартовий екран
#define START_CONTEXT SplashContext