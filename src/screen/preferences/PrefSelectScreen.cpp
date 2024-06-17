#include "PrefSelectScreen.h"

#include "meow/util/preferences/PrefUtil.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

#include "../WidgetCreator.h"

#include "meow/ui/widget/toggle/ToggleSwitch.h"
#include "meow/ui/widget/menu/item/ToggleItem.h"

PrefSelectScreen::PrefSelectScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK);
    layout->addWidget(navbar);

    _menu = new FixedMenu(ID_MENU, _display);
    layout->addWidget(_menu);
    _menu->setBackColor(COLOR_MENU_ITEM);
    _menu->setWidth(_display.width() - SCROLLBAR_WIDTH - 2);
    _menu->setHeight(_display.height() - NAVBAR_HEIGHT);
    _menu->setItemHeight((_display.height() - NAVBAR_HEIGHT) / 5);

    //
    MenuItem *bright_item = creator.getMenuItem(ITEM_ID_BRIGHT);
    _menu->addItem(bright_item);
    Label *bright_lbl = creator.getItemLabel(STR_BRIGHT, 4, 2);
    bright_item->setLbl(bright_lbl);
    //
    ToggleItem *mono_item = new ToggleItem(ITEM_ID_AUDIO_MONO, _display);
    _menu->addItem(mono_item);
    mono_item->setFocusBorderColor(COLOR_LIME);
    mono_item->setFocusBackColor(COLOR_FOCUS_BACK);
    mono_item->setChangingBorder(true);
    mono_item->setChangingBack(true);

    Label *mono_lbl = creator.getItemLabel(STR_AUDIO_MONO, 4, 2);
    mono_item->setLbl(mono_lbl);

    ToggleSwitch *toggle_mono = new ToggleSwitch(ID_TOGGLE, _display);
    mono_item->setToggle(toggle_mono);
    toggle_mono->setWidth(40);
    toggle_mono->setHeight(20);
    toggle_mono->setCornerRadius(7);

    PrefUtil pref;
    String mono_mode = pref.get(STR_PREF_MONO_AUDIO);
    if (mono_mode == "" || mono_mode == "0")
        toggle_mono->off();
    else
        toggle_mono->on();
    //
    MenuItem *watch_item = creator.getMenuItem(ITEM_ID_WATCH);
    _menu->addItem(watch_item);
    Label *watch_lbl = creator.getItemLabel(STR_WATCH, 4, 2);
    watch_item->setLbl(watch_lbl);

    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    _scrollbar->setMax(_menu->getSize());
}

void PrefSelectScreen::loop()
{
}

void PrefSelectScreen::update()
{
    if (_input.isReleased(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 500);
        ok();
    }
    else if (_input.isReleased(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 500);
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_input.isHolded(Input::PIN_UP))
    {
        _input.lock(Input::PIN_UP, 200);
        _menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_input.isHolded(Input::PIN_DOWN))
    {
        _input.lock(Input::PIN_DOWN, 200);
        _menu->focusDown();
        _scrollbar->scrollDown();
    }
}

void PrefSelectScreen::ok()
{
    uint16_t id = _menu->getCurrentItemID();

    if (id == ITEM_ID_AUDIO_MONO)
    {
        ToggleItem *toggle = (ToggleItem *)_menu->getCurrentItem();

        PrefUtil pref;
        bool force_mono = toggle->isOn();
        if (force_mono)
        {
            toggle->off();
            pref.set(STR_PREF_MONO_AUDIO, "0");
        }
        else
        {
            toggle->on();
            pref.set(STR_PREF_MONO_AUDIO, "1");
        }
    }
    else if (id == ITEM_ID_WATCH)
    {
        openScreenByID(ID_SCREEN_PREF_WATCH);
    }
    else if (id == ITEM_ID_BRIGHT)
    {
        openScreenByID(ID_SCREEN_PREF_BRIGHT);
    }
}
