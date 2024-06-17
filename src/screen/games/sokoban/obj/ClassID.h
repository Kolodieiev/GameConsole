// Перечислення, в якому зберігаються ідентифікатори типів ігрових об'єктів

#pragma once
#include <Arduino.h>

enum ClassID : int16_t
{
    CLASS_HERO = 0,
    CLASS_BOX,
    CLASS_BOX_POINT
};