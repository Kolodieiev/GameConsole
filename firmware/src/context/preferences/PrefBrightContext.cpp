#include "PrefBrightContext.h"

#include "meow/manager/settings/SettingsManager.h"
#include "meow/util/display/DisplayUtil.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

PrefBrightContext::PrefBrightContext(GraphicsDriver &display) : IContext(display)
{
    SettingsManager settings;
    String bright = settings.get(STR_PREF_BRIGHT);

    if (bright.equals(""))
        _old_bright = 100;
    else
        _old_bright = atoi(bright.c_str());

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
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

void PrefBrightContext::loop()
{
}

void PrefBrightContext::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);

        SettingsManager settings;
        settings.set(STR_PREF_BRIGHT, String(_progress->getProgress()).c_str());
        openContextByID(ID_CONTEXT_PREF_SEL);
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);

        DisplayUtil displ_util;
        displ_util.setBrightness(_old_bright);
        openContextByID(ID_CONTEXT_PREF_SEL);
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, 100);

        uint16_t cur_progress = _progress->getProgress();

        if (cur_progress < MAX_BRIGHT)
        {
            cur_progress += BRIGHT_STEP;
            _progress->setProgress(cur_progress);
            DisplayUtil displ_util;
            displ_util.setBrightness(cur_progress);
        }
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, 100);

        uint16_t cur_progress = _progress->getProgress();
        if (cur_progress > BRIGHT_STEP * 2)
        {
            cur_progress -= BRIGHT_STEP;
            _progress->setProgress(cur_progress);
            DisplayUtil displ_util;
            displ_util.setBrightness(cur_progress);
        }
    }
}