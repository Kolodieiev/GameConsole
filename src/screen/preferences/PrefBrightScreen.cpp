#include "PrefBrightScreen.h"

#include "meow/util/preferences/PrefUtil.h"
#include "meow/util/display/DisplayUtil.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

PrefBrightScreen::PrefBrightScreen(GraphicsDriver &display) : IScreen(display)
{
    PrefUtil pref;
    String bright = pref.get(STR_PREF_BRIGHT);

    if (bright.equals(""))
        _old_bright = 100;
    else
        _old_bright = atoi(bright.c_str());

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    //
    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SAVE, "", STR_BACK);
    layout->addWidget(navbar);
    //
    Label *header_lbl = new Label(ID_HEADER, _display);
    layout->addWidget(header_lbl);
    header_lbl->setText(STR_BRIGHT);
    header_lbl->setWidth(_display.width());
    header_lbl->setBackColor(COLOR_MAIN_BACK);
    header_lbl->setTextColor(TFT_WHITE);
    header_lbl->setAlign(IWidget::ALIGN_CENTER);
    header_lbl->setGravity(IWidget::GRAVITY_CENTER);
    //
    _progress = new ProgressBar(ID_PROGRESSBAR, _display);
    layout->addWidget(_progress);
    _progress->setBackColor(TFT_BLACK);
    _progress->setProgressColor(TFT_ORANGE);
    _progress->setBorderColor(TFT_WHITE);
    _progress->setMax(MAX_BRIGHT);
    _progress->setWidth(_display.width() - 5 * 8);
    _progress->setHeight(20);
    _progress->setProgress(_old_bright);
    _progress->setPos((_display.width() - _progress->getWidth()) / 2, 40);
}

void PrefBrightScreen::loop()
{
}

void PrefBrightScreen::update()
{
    if (_input.isReleased(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 500);

        PrefUtil pref;
        pref.set(STR_PREF_BRIGHT, String(_progress->getProgress()).c_str());
        openScreenByID(ID_SCREEN_PREF_SEL);
    }
    else if (_input.isReleased(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 500);

        DisplayUtil::setBrightness(_old_bright);
        openScreenByID(ID_SCREEN_PREF_SEL);
    }
    else if (_input.isHolded(Input::PIN_UP))
    {
        _input.lock(Input::PIN_UP, 100);

        uint16_t cur_progress = _progress->getProgress();

        if (cur_progress < MAX_BRIGHT)
        {
            cur_progress += BRIGHT_STEP;
            _progress->setProgress(cur_progress);
            DisplayUtil::setBrightness(cur_progress);
        }
    }
    else if (_input.isHolded(Input::PIN_DOWN))
    {
        _input.lock(Input::PIN_DOWN, 100);

        uint16_t cur_progress = _progress->getProgress();
        if (cur_progress > BRIGHT_STEP * 2)
        {
            cur_progress -= BRIGHT_STEP;
            _progress->setProgress(cur_progress);
            DisplayUtil::setBrightness(cur_progress);
        }
    }
}