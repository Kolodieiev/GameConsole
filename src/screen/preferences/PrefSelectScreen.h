#pragma once
#include <Arduino.h>

#include "meow/ui/screen/IScreen.h"
#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"

using namespace meow;

class PrefSelectScreen : public IScreen
{

public:
    PrefSelectScreen(GraphicsDriver &display);
    virtual ~PrefSelectScreen() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 2,
        ID_MENU,
        ID_SCROLLBAR,
        ID_TOGGLE,
    };

    enum ItemID : uint8_t
    {
        ITEM_ID_BRIGHT = 1,
        ITEM_ID_AUDIO_MONO,
        ITEM_ID_WATCH,
    };

    FixedMenu *_menu;
    ScrollBar *_scrollbar;
    

    void ok();
};