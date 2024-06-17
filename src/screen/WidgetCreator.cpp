#include "WidgetCreator.h"
#include "./resources/const.h"
#include "./resources/color.h"

WidgetCreator::WidgetCreator(GraphicsDriver &display) : _display{display} {}

EmptyLayout *WidgetCreator::getEmptyLayout()
{
    EmptyLayout *layout = new EmptyLayout(1, _display);
    layout->setBackColor(COLOR_MAIN_BACK);
    layout->setWidth(_display.width());
    layout->setHeight(_display.height());
    return layout;
}

Label *WidgetCreator::getDisplayDescription(uint16_t id, const char *text)
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

Label *WidgetCreator::getItemLabel(const char *text, uint8_t font_id, uint8_t text_size)
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

MenuItem *WidgetCreator::getMenuItem(uint16_t id)
{
    MenuItem *item = new MenuItem(id, _display);
    item->setFocusBorderColor(COLOR_LIME);
    item->setFocusBackColor(COLOR_FOCUS_BACK);
    item->setBackColor(COLOR_MENU_ITEM);
    item->setChangingBorder(true);
    item->setChangingBack(true);
    return item;
}

NavBar *WidgetCreator::getNavbar(uint16_t id, const char *left, const char *middle, const char *right)
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

DynamicMenu *WidgetCreator::getDynamicMenu(uint16_t id, IItemsLoader *loader)
{
    DynamicMenu *menu = new DynamicMenu(loader, id, _display);
    menu->setBackColor(TFT_BLACK);
    menu->setWidth(_display.width());
    menu->setHeight(_display.height() - NAVBAR_HEIGHT);
    return menu;
}
