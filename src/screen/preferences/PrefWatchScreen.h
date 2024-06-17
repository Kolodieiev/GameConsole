#pragma once
#include <Arduino.h>

#include "meow/ui/screen/IScreen.h"
#include "meow/lib/DS3231/DS3231.h"

using namespace meow;

class PrefWatchScreen : public IScreen
{

public:
    PrefWatchScreen(GraphicsDriver &display);
    virtual ~PrefWatchScreen() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 2,
        ID_HEADER,
        // Watch
        ID_HOURS_SPIN,
        ID_MINUTES_SPIN,
        ID_DAY_SPIN,
        ID_MONTH_SPIN,
        ID_YEAR_SPIN,
    };

    const uint8_t MAX_SPIN_POS{4};

    DS3231 _watch;
    DS3231DateTime _cur_date_time;
    bool _watch_inited{false};
    uint8_t _curent_focus_pos{0};

    void changeCurDateTime(uint16_t widget_id, uint16_t value);
};
