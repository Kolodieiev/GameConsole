#pragma once
#include <Arduino.h>

#include "meow/ui/context/IContext.h"

#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"

using namespace meow;

class MenuContext : public IContext
{
public:
    MenuContext(GraphicsDriver &display);
    virtual ~MenuContext();

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_MENU = 1,
        ID_SCROLLBAR,
    };

    FixedMenu *_menu;
    ScrollBar *_scrollbar;

    static uint8_t _last_sel_item_pos;

    std::vector<IWidget *> _bin;

    void up();
    void down();
    void ok();
};
