#pragma once
#include <Arduino.h>
#include "../sd/SdUtil.h"

namespace meow
{
    class PrefUtil
    {
    public:
        bool set(const char *pref_name, const char *value);
        String get(const char *pref_name);

    private:
        File *loadPrefFile(const char *file_name, const char *mode);
    };
}