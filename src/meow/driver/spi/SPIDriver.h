#pragma once
#include <Arduino.h>
#include <SPI.h>

namespace meow
{
    class SPIDriver
    {
    public:
        // SPIDriver();

    private:
    };

    extern SPIClass SD_SPI;
    extern SPIClass EXT_SPI;
}