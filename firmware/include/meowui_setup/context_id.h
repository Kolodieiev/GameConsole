#pragma once
#include <Arduino.h>

// -------------------------------- Додай ID екранів
namespace meow
{
    enum ContextID : uint8_t
    {
        ID_CONTEXT_SPLASH = 0,
        ID_CONTEXT_HOME,
        ID_CONTEXT_MENU,
        ID_CONTEXT_TORCH,
        ID_CONTEXT_GAMES,
        ID_CONTEXT_MP3,
        ID_CONTEXT_FILES,
        ID_CONTEXT_FIRMWARE,
        ID_CONTEXT_CALC,
        ID_CONTEXT_READER,
        // Екрани налаштувань
        ID_CONTEXT_PREF_SEL,
        ID_CONTEXT_PREF_BRIGHT,
        ID_CONTEXT_PREF_WATCH,
        ID_CONTEXT_PREF_WALL,
        ID_CONTEXT_PREF_FILE_SERVER,
        // Ігрові екрани
        ID_CONTEXT_SNAKE,
        ID_CONTEXT_SOKOBAN,
        ID_CONTEXT_TEST_SERVER,
        //
    };
}
