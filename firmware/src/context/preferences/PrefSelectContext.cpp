#include "PrefSelectContext.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

#include "../WidgetCreator.h"

#include "meow/ui/widget/toggle/ToggleSwitch.h"
#include "meow/ui/widget/menu/item/ToggleItem.h"

PrefSelectContext::PrefSelectContext(GraphicsDriver &display) : IContext(display)
{
    if (!_settings.isSdMounted())
    {
        showSDErrTmpl();
        return;
    }

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    _menu = new FixedMenu(ID_MENU, _display);
    layout->addWidget(_menu);
    _menu->setBackColor(COLOR_MENU_ITEM);
    _menu->setWidth(D_WIDTH - SCROLLBAR_WIDTH - 2);
    _menu->setHeight(D_HEIGHT);
    _menu->setItemHeight((_menu->getHeight() - 2) / 5);

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

    SettingsManager pref;
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
    //
    MenuItem *file_server_item = creator.getMenuItem(ITEM_ID_FILE_SERVER);
    _menu->addItem(file_server_item);
    Label *file_server_lbl = creator.getItemLabel(STR_FILE_SERVER, 4, 2);
    file_server_item->setLbl(file_server_lbl);

    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(D_HEIGHT);
    _scrollbar->setPos(D_WIDTH - SCROLLBAR_WIDTH, 0);
    _scrollbar->setMax(_menu->getSize());
}

void PrefSelectContext::showSDErrTmpl()
{
    _mode = MODE_SD_UNCONN;
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    Label *err_lbl = new Label(ID_ERR_LBL, _display);
    layout->addWidget(err_lbl);
    err_lbl->setText(STR_SD_ERR);
    err_lbl->setAlign(IWidget::ALIGN_CENTER);
    err_lbl->setGravity(IWidget::GRAVITY_CENTER);
    err_lbl->setBackColor(COLOR_MAIN_BACK);
    err_lbl->setWidth(D_WIDTH);
    err_lbl->setHeight(D_HEIGHT);
}

void PrefSelectContext::loop()
{
}

void PrefSelectContext::update()
{
    if (_mode == MODE_SD_UNCONN)
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            openContextByID(ID_CONTEXT_MENU);
        }

        return;
    }

    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, CLICK_LOCK);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
        openContextByID(ID_CONTEXT_MENU);
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, HOLD_LOCK);
        _menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
        _menu->focusDown();
        _scrollbar->scrollDown();
    }
}

void PrefSelectContext::ok()
{
    uint16_t id = _menu->getCurrentItemID();

    if (id == ITEM_ID_AUDIO_MONO)
    {
        ToggleItem *toggle = (ToggleItem *)_menu->getCurrentItem();

        bool force_mono = toggle->isOn();
        if (force_mono)
        {
            if (_settings.set(STR_PREF_MONO_AUDIO, "0"))
                toggle->off();
        }
        else
        {
            if (_settings.set(STR_PREF_MONO_AUDIO, "1"))
                toggle->on();
        }
    }
    else if (id == ITEM_ID_WATCH)
    {
        openContextByID(ID_CONTEXT_PREF_WATCH);
    }
    else if (id == ITEM_ID_BRIGHT)
    {
        openContextByID(ID_CONTEXT_PREF_BRIGHT);
    }
    else if (id == ITEM_ID_FILE_SERVER)
    {
        openContextByID(ID_CONTEXT_PREF_FILE_SERVER);
    }
}
