#include "TorchContext.h"

#include "meow/manager/settings/SettingsManager.h"
#include "meow/util/display/DisplayUtil.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
#include "../resources/string.h"

TorchContext::TorchContext(GraphicsDriver &display) : IContext{display}
{
    SettingsManager settings;
    String bright = settings.get(STR_PREF_BRIGHT);

    if (bright.equals(""))
        _display_brightness = 100;
    else
        _display_brightness = atoi(bright.c_str());

    _torch_brightness = _display_brightness;
    DisplayUtil displ_util;
    displ_util.setBrightness(_torch_brightness);

    EmptyLayout *layout = new EmptyLayout(1, display);
    layout->setBackColor(TFT_RED);
    layout->setWidth(display.width());
    layout->setHeight(display.height());

    setLayout(layout);
}

void TorchContext::loop()
{
}

void TorchContext::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);
        _is_light_on = !_is_light_on;

        DisplayUtil displ_util;

        if (!_is_light_on)
            displ_util.setBrightness(0);
        else
            displ_util.setBrightness(_torch_brightness);
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);

        DisplayUtil displ_util;
        displ_util.setBrightness(_display_brightness);
        openContextByID(ID_CONTEXT_HOME);
    }
    else if (_input.isReleased(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, 500);
        getLayout()->setBackColor(TFT_WHITE);
    }
    else if (_input.isReleased(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, 500);
        getLayout()->setBackColor(TFT_RED);
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, 50);

        if (_torch_brightness < 240)
        {
            _torch_brightness += 10;
            DisplayUtil displ_util;
            displ_util.setBrightness(_torch_brightness);
        }
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, 50);

        if (_torch_brightness > 20)
        {
            _torch_brightness -= 10;
            DisplayUtil displ_util;
            displ_util.setBrightness(_torch_brightness);
        }
    }
}
