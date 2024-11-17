#include "WidgetCreator.h"
#include "./resources/const.h"
#include "./resources/color.h"

WidgetCreator::WidgetCreator(GraphicsDriver &display) : _display{display} {}

EmptyLayout *WidgetCreator::getEmptyLayout()
{
    try
    {
        EmptyLayout *layout = new EmptyLayout(1, _display);
        layout->setBackColor(COLOR_MAIN_BACK);
        layout->setWidth(_display.width());
        layout->setHeight(_display.height());
        return layout;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

Label *WidgetCreator::getDisplayDescription(uint16_t id, const char *text)
{
    try
    {
        Label *display_descr = new Label(1, _display);
        display_descr->setText(text);
        display_descr->setWidth(_display.width());
        display_descr->setHeight(DISPLAY_DESCRIPT_HEIGHT);
        display_descr->setPos(0, 0);
        display_descr->setTextColor(TFT_WHITE);
        display_descr->setBackColor(TFT_BLACK);
        display_descr->setAlign(IWidget::ALIGN_CENTER);
        display_descr->setGravity(IWidget::GRAVITY_CENTER);

        return display_descr;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

Label *WidgetCreator::getItemLabel(const char *text, uint8_t font_id, uint8_t text_size)
{
    try
    {
        Label *item = new Label(1, _display);
        item->setText(text);
        item->setFontID(font_id);
        item->setTextSize(text_size);
        item->setGravity(IWidget::GRAVITY_CENTER);
        item->setTextOffset(3);
        item->setTickerInFocus(true);
        return item;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

MenuItem *WidgetCreator::getMenuItem(uint16_t id)
{
    try
    {
        MenuItem *item = new MenuItem(id, _display);
        item->setFocusBorderColor(COLOR_LIME);
        item->setFocusBackColor(COLOR_FOCUS_BACK);
        item->setBackColor(COLOR_MENU_ITEM);
        item->setChangingBorder(true);
        item->setChangingBack(true);
        return item;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

NavBar *WidgetCreator::getNavbar(uint16_t id, const char *left, const char *middle, const char *right)
{
    try
    {
        Label *sel_txt = new Label(1, _display);
        sel_txt->setText(left);
        sel_txt->setAlign(IWidget::ALIGN_CENTER);
        sel_txt->setGravity(IWidget::GRAVITY_CENTER);
        sel_txt->setTextColor(TFT_WHITE);
        sel_txt->setTextOffset(1);
        sel_txt->initWidthToFit();

        Label *mid_txt = sel_txt->clone(2);
        mid_txt->setText(middle);
        mid_txt->initWidthToFit();

        Label *back_txt = sel_txt->clone(3);
        back_txt->setText(right);
        back_txt->initWidthToFit();

        NavBar *navbar = new NavBar(id, _display);
        navbar->setBackColor(COLOR_NAV_PANEL_BACK);
        navbar->setHeight(NAVBAR_HEIGHT);
        navbar->setWidth(_display.width());
        navbar->setPos(0, _display.height() - NAVBAR_HEIGHT);
        navbar->setWidgets(sel_txt, mid_txt, back_txt);

        return navbar;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

DynamicMenu *WidgetCreator::getDynamicMenu(uint16_t id, IItemsLoader *loader)
{
    try
    {
        DynamicMenu *menu = new DynamicMenu(loader, id, _display);
        menu->setBackColor(TFT_BLACK);
        menu->setWidth(_display.width());
        menu->setHeight(_display.height() - NAVBAR_HEIGHT);
        return menu;
    }
    catch (const std::bad_alloc &e)
    {
        log_e("%s", e.what());
        esp_restart();
    }
}

Label *WidgetCreator::getStatusMsgLable(uint16_t id, const char *text, uint8_t text_size)
{
    Label *lbl = new Label(id, _display);
    lbl->setText(text);
    lbl->setTextSize(text_size);
    lbl->setAlign(IWidget::ALIGN_CENTER);
    lbl->setGravity(IWidget::GRAVITY_CENTER);
    lbl->setBackColor(COLOR_MAIN_BACK);
    lbl->setWidth(_display.width());
    lbl->setHeight(_display.height() - NAVBAR_HEIGHT);

    return lbl;
}
