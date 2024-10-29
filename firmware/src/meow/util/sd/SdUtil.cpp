#include "SdUtil.h"
#include "../../setup/sd_setup.h"
#include "../../driver/spi/SPIDriver.h"

bool meow::SdUtil::_is_inited{false};

bool meow::SdUtil::begin()
{
    SD_SPI.begin(SD_PIN_SCLK, SD_PIN_MISO, SD_PIN_MOSI);
    spiDetachSS(SD_SPI.bus(), SS);
    spiSSClear(SD_SPI.bus());

    _is_inited = SD.begin(SD_PIN_CS, SD_SPI, SD_FREQUENCY, SD_MOUNTPOINT, SD_MAX_FILES);

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
