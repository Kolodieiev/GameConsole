#pragma once
#include <Arduino.h>
#include "meow/ui/screen/IScreen.h"
#include "meow/util/preferences/PrefUtil.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"
#include "meow/ui/widget/menu/FixedMenu.h"

using namespace meow;

class ReaderScreen : public IScreen
{

public:
    ReaderScreen(GraphicsDriver &display);
    virtual ~ReaderScreen();

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_PAGE,
        ID_SCROLL,
        ID_TIME,
        ID_PROGRESS,
    };

    enum Mode : uint8_t
    {
        MODE_CAT_SEL = 0,
        MODE_CAT_UPD,
        MODE_BOOK_SEL,
        MODE_BOOK_READING,
        MODE_LIST_MENU
    };

    enum BlMenuItemsID : uint8_t
    {
        ID_ITEM_DEL = 1,
        ID_ITEM_UPD,
    };

    const uint8_t MAX_CHAR_TO_READ{200};

    uint8_t _brightness;

    Label *_page;
    uint64_t _file_pos{0};

    void ok();
    void up();
    void down();
    void left();
    void right();

    void back();
    void backPressed();
};
