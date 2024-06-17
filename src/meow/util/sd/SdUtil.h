#pragma once
#include <Arduino.h>
#include <SD.h>

namespace meow
{
    class SdUtil
    {
    public:
        bool begin();
        void end();
        bool hasConnection();

    private:
        static bool _is_inited;
    };
}