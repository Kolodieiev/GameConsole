#include "PrefWatchScreen.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/spinbox/SpinBox.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

PrefWatchScreen::PrefWatchScreen(GraphicsDriver &display) : IScreen(display)
{
    _watch_inited = _watch.begin();

    if (!_watch_inited)
    {
        log_e("Помилка ініціалізації RTC");
        openScreenByID(ID_SCREEN_PREF_SEL);
        return;
    }

    if (!_watch.isRunning())
    {
        _cur_date_time.hour = 0;
        _cur_date_time.minute = 0;
        _cur_date_time.second = 0;
        _cur_date_time.month = 0;
        _cur_date_time.year = 0;
        _watch.setDateTime(_cur_date_time);
        _watch.enable();
    }

    _cur_date_time = _watch.getDateTime();

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    //
    SpinBox *hours = new SpinBox(ID_HOURS_SPIN, _display);
    layout->addWidget(hours);
    hours->setAlign(IWidget::ALIGN_CENTER);
    hours->setGravity(IWidget::GRAVITY_CENTER);
    hours->setHeight(34);
    hours->setWidth(7 * 10);
    hours->setBackColor(TFT_BLACK);
    hours->setTextColor(TFT_ORANGE);
    hours->setTextSize(2);
    hours->setFocusBorderColor(COLOR_LIME);
    hours->setChangingBorder(true);
    hours->setCornerRadius(5);
    hours->setMin(0);
    hours->setMax(23);
    hours->setValue(_cur_date_time.hour);
    hours->setPos((_display.width() - (hours->getWidth() * 2 + 5)) * 0.5, 100);

    SpinBox *minutes = hours->clone(ID_MINUTES_SPIN);
    layout->addWidget(minutes);
    minutes->setMin(0);
    minutes->setMax(59);
    minutes->setValue(_cur_date_time.minute);
    minutes->setPos(hours->getXPos() + hours->getWidth() + 5, hours->getYPos());

    SpinBox *day = hours->clone(ID_DAY_SPIN);
    layout->addWidget(day);
    day->setMin(1);
    day->setMax(31);
    day->setValue(_cur_date_time.day_of_month);
    day->setPos((_display.width() - (hours->getWidth() * 3 + 10)) * 0.5, hours->getYPos() + hours->getHeight() + 3);

    SpinBox *month = hours->clone(ID_MONTH_SPIN);
    layout->addWidget(month);
    month->setMin(1);
    month->setMax(12);
    month->setValue(_cur_date_time.month);
    month->setPos(day->getXPos() + day->getWidth() + 5, day->getYPos());

    SpinBox *year = hours->clone(ID_YEAR_SPIN);
    layout->addWidget(year);
    year->setMin(2020);
    year->setMax(3000);
    year->setValue(_cur_date_time.year);
    year->setPos(month->getXPos() + month->getWidth() + 5, day->getYPos());

    hours->setFocus();

    //
    Label *header_lbl = new Label(ID_HEADER, _display);
    layout->addWidget(header_lbl);
    header_lbl->setText(STR_WATCH);
    header_lbl->setWidth(_display.width());
    header_lbl->setBackColor(COLOR_MAIN_BACK);
    header_lbl->setTextColor(TFT_WHITE);
    header_lbl->setAlign(IWidget::ALIGN_CENTER);
    header_lbl->setGravity(IWidget::GRAVITY_CENTER);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SAVE, "", STR_BACK);
    layout->addWidget(navbar);
}

void PrefWatchScreen::loop()
{
}

void PrefWatchScreen::update()
{
    if (_input.isReleased(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 500);

        _watch.setDateTime(_cur_date_time);
        openScreenByID(ID_SCREEN_PREF_SEL);
    }
    else if (_input.isReleased(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 500);
        openScreenByID(ID_SCREEN_PREF_SEL);
    }
    else if (_input.isReleased(Input::PIN_LEFT))
    {
        _input.lock(Input::PIN_LEFT, 200);

        SpinBox *spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->removeFocus();

        if (_curent_focus_pos > 0)
            _curent_focus_pos--;
        else
            _curent_focus_pos = MAX_SPIN_POS;

        spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->setFocus();
    }
    else if (_input.isReleased(Input::PIN_RIGHT))
    {
        _input.lock(Input::PIN_RIGHT, 200);

        SpinBox *spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->removeFocus();

        if (_curent_focus_pos == MAX_SPIN_POS)
            _curent_focus_pos = 0;
        else
            _curent_focus_pos++;

        spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->setFocus();
    }
    else if (_input.isHolded(Input::PIN_UP))
    {
        _input.lock(Input::PIN_UP, 150);
        SpinBox *spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->up();
        changeCurDateTime(spinBox->getID(), spinBox->getValue());
    }
    else if (_input.isHolded(Input::PIN_DOWN))
    {
        _input.lock(Input::PIN_DOWN, 150);
        SpinBox *spinBox = (SpinBox *)getLayout()->getWidgetByPos(_curent_focus_pos);
        spinBox->down();
        changeCurDateTime(spinBox->getID(), spinBox->getValue());
    }
}

void PrefWatchScreen::changeCurDateTime(uint16_t widget_id, uint16_t value)
{
    if (widget_id == ID_HOURS_SPIN)
        _cur_date_time.hour = value;
    else if (widget_id == ID_MINUTES_SPIN)
        _cur_date_time.minute = value;
    else if (widget_id == ID_DAY_SPIN)
        _cur_date_time.day_of_month = value;
    else if (widget_id == ID_MONTH_SPIN)
        _cur_date_time.month = value;
    else if (widget_id == ID_YEAR_SPIN)
        _cur_date_time.year = value;
}
