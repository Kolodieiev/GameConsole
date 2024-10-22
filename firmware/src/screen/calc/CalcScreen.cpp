#include "CalcScreen.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

CalcScreen::CalcScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
}

void CalcScreen::loop()
{
}

void CalcScreen::update()
{
    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1500);
        // showContextMenu();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 1500);
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, 130);
        _keyboard->focusUp();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, 130);
        _keyboard->focusDown();
    }
    else if (_input.isHolded(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, 130);
        _keyboard->focusRight();
    }
    else if (_input.isHolded(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, 130);
        _keyboard->focusLeft();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 200);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 300);
        back();
    }
}

void CalcScreen::ok()
{
}

void CalcScreen::back()
{
    //   if (_mode == MODE_CONTEXT_MENU)
    //     hideContextMenu();
    // else
    _calc_txt->removeLastChar();
}
