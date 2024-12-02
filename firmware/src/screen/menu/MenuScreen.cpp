#include "MenuScreen.h"

#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/const.h"
//
#include "./res/headphones.h"
#include "./res/joystick.h"
#include "./res/sd.h"
#include "./res/settings.h"
#include "./res/chip.h"
#include "./res/calc.h"
#include "./res/book.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
#include "meow/ui/widget/navbar/NavBar.h"
#include "meow/ui/widget/menu/item/MenuItem.h"

MenuScreen::MenuScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};
    //
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    //
    _menu = new FixedMenu(ID_MENU, _display);
    layout->addWidget(_menu);
    _menu->setBackColor(COLOR_MENU_ITEM);
    _menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _menu->setHeight(_display.height() - NAVBAR_HEIGHT - 1);
    _menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / 6);
    //
    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    _scrollbar->setBackColor(COLOR_MAIN_BACK);
    //
    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK);
    layout->addWidget(navbar);

    // Музика
    MenuItem *mp3_item = creator.getMenuItem(ID_SCREEN_MP3);
    _menu->addItem(mp3_item);

    Image *mp3_img = new Image(1, _display);
    mp3_item->setImg(mp3_img);
    mp3_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    mp3_img->init(35, 35);
    mp3_img->setSrc(HEADPHONES_IMG);

    Label *mp3_lbl = creator.getItemLabel(STR_MUSIC_ITEM, 4, 2);
    mp3_item->setLbl(mp3_lbl);

    // // Калькулятор
    // MenuItem *calc_item = creator.getMenuItem(ID_SCREEN_CALC);
    // _menu->addItem(calc_item);

    // Image *calc_img = new Image(1, _display);
    // calc_item->setImg(calc_img);
    // calc_img->setTransparentColor(Image::TRANSPARENT_COLOR);
    // calc_img->init(35, 35);
    // calc_img->setSrc(CALC_IMG);

    // Label *calc_lbl = creator.getItemLabel(STR_CALC_ITEM, 4, 2);
    // calc_item->setLbl(calc_lbl);

    // Читалка
    MenuItem *read_item = creator.getMenuItem(ID_SCREEN_READER);
    _menu->addItem(read_item);

    Image *read_img = new Image(1, _display);
    read_item->setImg(read_img);
    read_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    read_img->init(35, 35);
    read_img->setSrc(BOOK_IMG);

    Label *read_lbl = creator.getItemLabel(STR_READER_ITEM, 4, 2);
    read_item->setLbl(read_lbl);

    // Ігри
    MenuItem *game_item = creator.getMenuItem(ID_SCREEN_GAMES);
    _menu->addItem(game_item);

    Image *game_img = new Image(1, _display);
    game_item->setImg(game_img);
    game_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    game_img->init(35, 35);
    game_img->setSrc(JOYSTICK_IMG);

    Label *game_lbl = creator.getItemLabel(STR_GAME_ITEM, 4, 2);
    game_item->setLbl(game_lbl);

    // Файли
    MenuItem *files_item = creator.getMenuItem(ID_SCREEN_FILES);
    _menu->addItem(files_item);

    Image *files_img = new Image(1, _display);
    files_item->setImg(files_img);
    files_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    files_img->init(35, 35);
    files_img->setSrc(SD_IMG);

    Label *files_lbl = creator.getItemLabel(STR_FILES_ITEM, 4, 2);
    files_item->setLbl(files_lbl);

    // Налаштування
    MenuItem *pref_item = creator.getMenuItem(ID_SCREEN_PREF_SEL);
    _menu->addItem(pref_item);

    Image *pref_img = new Image(1, _display);
    pref_item->setImg(pref_img);
    pref_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    pref_img->init(35, 35);
    pref_img->setSrc(SETTINGS_IMG);

    Label *pref_lbl = creator.getItemLabel(STR_PREFERENCES_ITEM, 4, 2);
    pref_item->setLbl(pref_lbl);

    // Прошика
    MenuItem *firm_item = creator.getMenuItem(ID_SCREEN_FIRMWARE);
    _menu->addItem(firm_item);

    Image *firm_img = new Image(1, _display);
    firm_item->setImg(firm_img);
    firm_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    firm_img->init(35, 35);
    firm_img->setSrc(CHIP_IMG);

    Label *firm_lbl = creator.getItemLabel(STR_FIRMWARE_ITEM, 4, 2);
    firm_item->setLbl(firm_lbl);
    //

    _bin.reserve(_menu->getSize());
    _bin.push_back(mp3_img);
    _bin.push_back(read_img);
    _bin.push_back(game_img);
    _bin.push_back(files_img);
    _bin.push_back(pref_img);
    _bin.push_back(firm_img);

    _scrollbar->setMax(_menu->getSize());
}

MenuScreen::~MenuScreen()
{
    for (auto b_it = _bin.begin(), e_it = _bin.end(); b_it != e_it; ++b_it)
        delete *b_it;
}

void MenuScreen::loop()
{
}

void MenuScreen::update()
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
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);
        openScreenByID(ID_SCREEN_HOME);
    }
}

void MenuScreen::up()
{
    _menu->focusUp();
    _scrollbar->scrollUp();
}

void MenuScreen::down()
{
    _menu->focusDown();
    _scrollbar->scrollDown();
}

void MenuScreen::ok()
{
    uint16_t id = _menu->getCurrentItemID();
    openScreenByID((ScreenID)id);
}
