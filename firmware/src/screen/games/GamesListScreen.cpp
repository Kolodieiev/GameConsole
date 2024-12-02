#include "GamesListScreen.h"
#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/const.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
#include "meow/ui/widget/navbar/NavBar.h"
#include "meow/ui/widget/menu/item/MenuItem.h"

#include "./icons/snake_ico.h"
#include "./icons/sokoban_ico.h"

GamesListScreen::GamesListScreen(GraphicsDriver &display) : IScreen(display)
{
    //------ Налаштування зовнішнього вигляду віджетів

    WidgetCreator creator{_display};
    //
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    //
    _menu = new FixedMenu(ID_MENU, _display);
    layout->addWidget(_menu);
    _menu->setBackColor(COLOR_MENU_ITEM);
    _menu->setWidth(_display.width() - SCROLLBAR_WIDTH - 2);
    _menu->setHeight(_display.height() - NAVBAR_HEIGHT - 2);
    _menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / 4);

    //
    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    //
    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK);
    layout->addWidget(navbar);

    //---------------------------
    MenuItem *sokoban_item = creator.getMenuItem(ID_SCREEN_SOKOBAN);
    _menu->addItem(sokoban_item);

    Image *soko_img = new Image(1, _display);
    sokoban_item->setImg(soko_img);
    soko_img->init(32, 32);
    soko_img->setCornerRadius(5);
    soko_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    soko_img->setSrc(ICO_SOKOBAN);

    Label *soko_lbl = creator.getItemLabel(STR_SOKOBAN_ITEM, 4, 2);
    sokoban_item->setLbl(soko_lbl);
    soko_lbl->setTickerInFocus(true);
    //

    //---------------------------------
    _bin.reserve(_menu->getSize());
    _bin.push_back(soko_img);

    _scrollbar->setMax(_menu->getSize());
}

GamesListScreen::~GamesListScreen()
{
    for (auto b_it = _bin.begin(), e_it = _bin.end(); b_it != e_it; ++b_it)
        delete *b_it;
}

void GamesListScreen::loop()
{
}

void GamesListScreen::update()
{
    if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, 200);
        up();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, 200);
        down();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);

        openScreenByID((ScreenID)_menu->getCurrentItemID());
    }
}

void GamesListScreen::up()
{
    _menu->focusUp();
    _scrollbar->scrollUp();
}

void GamesListScreen::down()
{
    _menu->focusDown();
    _scrollbar->scrollDown();
}

void GamesListScreen::ok()
{
    uint8_t screen_id = _menu->getCurrentItemID();

    if (screen_id == ID_SCREEN_SNAKE)
        return; // Гру ще не реалізовано

    openScreenByID((ScreenID)screen_id); // Ігровий екран буде відкриватися без додаткової перевірки.
}
