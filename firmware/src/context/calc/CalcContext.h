#pragma once
#include <Arduino.h>
#include "meow/ui/context/IContext.h"
#include "meow/ui/widget/text/TextBox.h"
#include "meow/ui/widget/keyboard/Keyboard.h"

using namespace meow;

class CalcContext : public IContext
{
public:
    CalcContext(GraphicsDriver &display);
    virtual ~CalcContext() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_KEYBOARD,
    };

    enum BTN_ID : uint8_t
    {
        ID_BTN_CLR = 1,
        ID_BTN_COMMA,
        ID_BTN_PLUS,
        ID_BTN_MINUS,
        ID_BTN_MULT,
        ID_BTN_DIV,
        ID_BTN_PERC,
        ID_BTN_PLUS_MIN,
        ID_BTN_RESULT,
        ID_BTN_SQR,
        ID_BTN_POW,
        //
        ID_BTN_0,
        ID_BTN_1,
        ID_BTN_2,
        ID_BTN_3,
        ID_BTN_4,
        ID_BTN_5,
        ID_BTN_6,
        ID_BTN_7,
        ID_BTN_8,
        ID_BTN_9,
    };

    bool _has_minus{false};
    TextBox *_calc_txt;
    Keyboard *_keyboard;
    
    void ok();
    void back();
};