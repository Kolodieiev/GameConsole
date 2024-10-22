#include "HomeScreen.h"
//
#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/ico/battery.h"
//
#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
#include "meow/ui/widget/navbar/NavBar.h"
//
#include "meow/util/bmp/BmpUtil.h"
#include "meow/util/sd/SdUtil.h"

HomeScreen::HomeScreen(GraphicsDriver &display) : IScreen{display}
{
    _watch_inited = _watch.begin();

    if (!_watch_inited)
        log_e("Помилка ініціалізації RTC");

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

    SdUtil sd;
    sd.end();

    _bat_cap_lbl = new Label(ID_BAT_LVL, _display);
    layout->addWidget(_bat_cap_lbl);
    _bat_cap_lbl->setWidth(56);
    _bat_cap_lbl->setHeight(35);
    _bat_cap_lbl->setAlign(Label::ALIGN_CENTER);
    _bat_cap_lbl->setGravity(Label::GRAVITY_CENTER);
    _bat_cap_lbl->setPos(_display.width() - 60, 3);

    _bat_ico = new Image(1, _display);
    _bat_cap_lbl->setBackImg(_bat_ico);
    _bat_ico->init(56, 35);
    _bat_ico->setSrc(ICO_BATTERY);
    _bat_ico->setTransparentColor(_bat_ico->TRANSPARENT_COLOR);

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

    _date_lbl = new Label(ID_DATE_LBL, _display);
    layout->addWidget(_date_lbl);
    _date_lbl->setText("00.00.0000");
    _date_lbl->setAlign(Label::ALIGN_CENTER);
    _date_lbl->setGravity(Label::GRAVITY_CENTER);
    _date_lbl->setPos(5, _time_lbl->getHeight() + 3);
    _date_lbl->initWidthToFit();

    _day_lbl = _date_lbl->clone(ID_DAY_LBL);
    layout->addWidget(_day_lbl);
    _day_lbl->setText("------");
    _day_lbl->setPos(5, _date_lbl->getYPos() + _date_lbl->getHeight() + 3);
    _day_lbl->initWidthToFit();

    updateWatch();

    //

    if (!_wallpaper_ptr)
    {
        _bat_ico->setBackColor(COLOR_MAIN_BACK);
        _time_lbl->setBackColor(COLOR_MAIN_BACK);
        _date_lbl->setBackColor(COLOR_MAIN_BACK);
        _day_lbl->setBackColor(COLOR_MAIN_BACK);
    }
    else
    {
        uint16_t back_bat_color = *(_wallpaper_ptr + _bat_cap_lbl->getYPos() * bmp.width + _bat_cap_lbl->getXPos());

        _bat_ico->setBackColor(back_bat_color);

        uint16_t back_lbl_color = *(_wallpaper_ptr + _time_lbl->getYPos() * bmp.width + _time_lbl->getXPos());

        _time_lbl->setBackColor(back_lbl_color);
        _date_lbl->setBackColor(back_lbl_color);
        _day_lbl->setBackColor(back_lbl_color);
    }

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_MENU, "", STR_TORCH);
    layout->addWidget(navbar);
}

HomeScreen::~HomeScreen()
{
    delete _bat_ico;

    if (_wallpaper_ptr)
        free((void *)_wallpaper_ptr);
}

void HomeScreen::loop()
{
}

void HomeScreen::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);
        openScreenByID(ID_SCREEN_MENU);
        return;
    }

    if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);
        openScreenByID(ID_SCREEN_TORCH);
        return;
    }

    if (millis() - _upd_timer > UPD_DISPLAY_INTERVAL)
    {
        _upd_timer = millis();
        updateBatCap();
        updateWatch();
    }
}

void HomeScreen::updateBatCap()
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

void HomeScreen::updateWatch()
{
    if (!_watch_inited)
        return;

    DS3231DateTime date_time = _watch.getDateTime();

    if (date_time == _temp_date_time)
        return;

    _temp_date_time = date_time;

    String temp_str;

    // Час
    if (date_time.hour < 10)
    {
        temp_str = "0";
        temp_str += String(date_time.hour);
        temp_str += ":";
    }
    else
    {
        temp_str = String(date_time.hour);
        temp_str += ":";
    }

    if (date_time.minute < 10)
    {
        temp_str += "0";
        temp_str += String(date_time.minute);
    }
    else
    {
        temp_str += String(date_time.minute);
    }

    _time_lbl->setText(temp_str);
    _time_lbl->updateWidthToFit();

    // Дата
    if (date_time.day_of_month < 10)
    {
        temp_str = "0";
        temp_str += String(date_time.day_of_month);
        temp_str += ".";
    }
    else
    {
        temp_str = String(date_time.day_of_month);
        temp_str += ".";
    }

    if (date_time.month < 10)
    {
        temp_str += "0";
        temp_str += String(date_time.month);
        temp_str += ".";
    }
    else
    {
        temp_str += String(date_time.month);
        temp_str += ".";
    }

    temp_str += String(date_time.year);

    _date_lbl->setText(temp_str);
    _date_lbl->updateWidthToFit();

    // День тижня
    _day_lbl->setText(DAY_OF_WEEK[date_time.dayOfWeek()]);
    _day_lbl->updateWidthToFit();
}
