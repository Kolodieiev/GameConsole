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
    if (_input.isPressed(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 1500);
        // showContextMenu();
    }
    else if (_input.isPressed(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 1500);
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_input.isHolded(Input::PIN_UP))
    {
        _input.lock(Input::PIN_UP, 130);
        _keyboard->focusUp();
    }
    else if (_input.isHolded(Input::PIN_DOWN))
    {
        _input.lock(Input::PIN_DOWN, 130);
        _keyboard->focusDown();
    }
    else if (_input.isHolded(Input::PIN_RIGHT))
    {
        _input.lock(Input::PIN_RIGHT, 130);
        _keyboard->focusRight();
    }
    else if (_input.isHolded(Input::PIN_LEFT))
    {
        _input.lock(Input::PIN_LEFT, 130);
        _keyboard->focusLeft();
    }
    else if (_input.isReleased(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 200);
        ok();
    }
    else if (_input.isReleased(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 300);
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
