#pragma once
#include <Arduino.h>

#include "meow/ui/context/IContext.h"
#include "meow/ui/widget/text/Label.h"

#include "meow/lib/DS3231/DS3231.h"

using namespace meow;

class HomeContext : public IContext
{
public:
    HomeContext(GraphicsDriver &display);
    virtual ~HomeContext();

protected:
    virtual void loop() override;
    virtual void update() override;
    //
private:
    enum Widget_ID : uint8_t
    {
        ID_WALLPAPER = 1,
        ID_BAT_LVL,
        ID_TIME_LBL,
        ID_DATE_LBL,
        ID_DAY_LBL,
    };

    const unsigned long UPD_DISPLAY_INTERVAL{5000};
    const uint8_t PIN_VOLT_MEASH{18};
    // const uint8_t PIN_VOLT_MEASH{17}; У виправленій схемі

    Image *_bat_ico;
    const uint16_t *_wallpaper_ptr{nullptr};
    unsigned long _upd_timer{0};

    DS3231 _watch;
    bool _watch_inited{false};
    DS3231DateTime _temp_date_time;

    Label *_bat_cap_lbl{nullptr};
    Label *_time_lbl{nullptr};
    Label *_date_lbl{nullptr};
    Label *_day_lbl{nullptr};

    void updateBatCap(bool full_redraw);
    void updateWatch(bool full_redraw);
};
