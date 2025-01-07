#include "HomeContext.h"
//
#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/ico/battery.h"
//
#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
//
#include "meow/util/bmp/BmpUtil.h"

HomeContext::HomeContext(GraphicsDriver &display) : IContext{display}
{
    _watch_inited = _watch.begin();

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    BmpUtil util;
    BmpData bmp = util.loadBmp("/wall/wallpaper.bmp");

    if (bmp.is_loaded)
    {
        _wallpaper_ptr = bmp.data_ptr;

        Image *wallpp_img = new Image(ID_WALLPAPER, _display);
        layout->addWidget(wallpp_img);
        wallpp_img->init(bmp.width, bmp.height);
        wallpp_img->setSrc(_wallpaper_ptr);
        wallpp_img->setPos(0, 0);
    }

    _bat_cap_lbl = new Label(ID_BAT_LVL, _display);
    layout->addWidget(_bat_cap_lbl);
    _bat_cap_lbl->setWidth(56);
    _bat_cap_lbl->setHeight(35);
    _bat_cap_lbl->setAlign(Label::ALIGN_CENTER);
    _bat_cap_lbl->setGravity(Label::GRAVITY_CENTER);
    _bat_cap_lbl->setPos(_display.width() - 60, 3);
    _bat_cap_lbl->setTransparency(true);

    _bat_ico = new Image(1, _display);
    _bat_cap_lbl->setBackImg(_bat_ico);
    _bat_ico->init(56, 35);
    _bat_ico->setSrc(ICO_BATTERY);
    _bat_ico->setTransparency(true);
    _bat_ico->setTransparentColor(Image::COLOR_TRANSPARENT);

    updateBatCap();

    //
    _time_lbl = new Label(ID_TIME_LBL, _display);
    layout->addWidget(_time_lbl);
    _time_lbl->setText(EMPTY_TIME);
    _time_lbl->setAlign(Label::ALIGN_CENTER);
    _time_lbl->setGravity(Label::GRAVITY_CENTER);
    _time_lbl->setFontID(2);
    _time_lbl->setTextSize(2);
    _time_lbl->initWidthToFit();
    _time_lbl->setPos(5, 0);
    _time_lbl->setTransparency(true);

    _date_lbl = new Label(ID_DATE_LBL, _display);
    layout->addWidget(_date_lbl);
    _date_lbl->setText("00.00.0000");
    _date_lbl->setAlign(Label::ALIGN_CENTER);
    _date_lbl->setGravity(Label::GRAVITY_CENTER);
    _date_lbl->setPos(5, _time_lbl->getHeight() + 3);
    _date_lbl->initWidthToFit();
    _date_lbl->setTransparency(true);

    _day_lbl = _date_lbl->clone(ID_DAY_LBL);
    layout->addWidget(_day_lbl);
    _day_lbl->setText("------");
    _day_lbl->setPos(5, _date_lbl->getYPos() + _date_lbl->getHeight() + 3);
    _day_lbl->initWidthToFit();
    _day_lbl->setTransparency(true);

    updateWatch();

    //
}

HomeContext::~HomeContext()
{
    delete _bat_ico;
    free((void *)_wallpaper_ptr);
}

void HomeContext::loop()
{
}

void HomeContext::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, CLICK_LOCK);
        openContextByID(ID_CONTEXT_MENU);
        return;
    }

    if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
        openContextByID(ID_CONTEXT_TORCH);
        return;
    }

    if (millis() - _upd_timer > UPD_DISPLAY_INTERVAL)
    {
        _upd_timer = millis();
        updateBatCap();
        updateWatch();
    }
}

void HomeContext::updateBatCap()
{
    float DIV_K{0.5827};
    const uint8_t READ_CYCLES_NUMBER = 128;

    float bat_voltage{0.0f};

    for (uint8_t i{0}; i < READ_CYCLES_NUMBER; ++i)
        bat_voltage += analogRead(PIN_VOLT_MEASH);

    bat_voltage /= READ_CYCLES_NUMBER;
    bat_voltage *= 3.3;
    bat_voltage /= 4095;
    bat_voltage /= DIV_K;

    String volt_str = String(bat_voltage);
    _bat_cap_lbl->setText(volt_str);
}

void HomeContext::updateWatch()
{
    if (!_watch_inited)
        return;

    DS3231DateTime date_time = _watch.getDateTime();

    if (date_time == _temp_date_time)
        return;

    _temp_date_time = date_time;

    _time_lbl->setText(_temp_date_time.timeToStr());
    _time_lbl->updateWidthToFit();

    _date_lbl->setText(_temp_date_time.dateToStr());
    _date_lbl->updateWidthToFit();

    // День тижня
    _day_lbl->setText(DAY_OF_WEEK[date_time.dayOfWeek()]);
    _day_lbl->updateWidthToFit();
}
