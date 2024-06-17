#include "SdUtil.h"
#include "meow/driver/spi/SPIDriver.h"

#define SD_CS_PIN 46 // Не використовується на платі

const char MOUNTPOINT[] = "/sd";

bool meow::SdUtil::_is_inited{false};

bool meow::SdUtil::begin()
{
    SD_SPI.begin();
    spiDetachSS(SD_SPI.bus(), SS);
    spiSSClear(SD_SPI.bus());

    _is_inited = SD.begin(SD_CS_PIN, SD_SPI, 80000000, MOUNTPOINT, 255U);

    if (!_is_inited)
        log_e("Помилка ініціалізації SD");

    return _is_inited;
}

void meow::SdUtil::end()
{
    SD.end();
    SD_SPI.end();
    _is_inited = false;
}

bool meow::SdUtil::hasConnection()
{
    if (!_is_inited)
    {
        if (!begin())
        {
            end();
            return false;
        }
    }

    return true;
}
