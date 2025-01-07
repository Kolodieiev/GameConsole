#include "MenuContext.h"

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
#include "meow/ui/widget/menu/item/MenuItem.h"

#define ICO_WH 35

uint8_t MenuContext::_last_sel_item_pos;

MenuContext::MenuContext(GraphicsDriver &display) : IContext(display)
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
    _menu->setHeight(_display.height());
    _menu->setItemHeight((_display.height() - 2) / 6);
    //
    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height());
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    _scrollbar->setBackColor(COLOR_MAIN_BACK);
    //
    // Музика
    MenuItem *mp3_item = creator.getMenuItem(ID_CONTEXT_MP3);
    _menu->addItem(mp3_item);

    Image *mp3_img = new Image(1, _display);
    mp3_item->setIco(mp3_img);
    mp3_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    mp3_img->init(ICO_WH, ICO_WH);
    mp3_img->setSrc(HEADPHONES_IMG);

    Label *mp3_lbl = creator.getItemLabel(STR_MUSIC_ITEM, 4, 2);
    mp3_item->setLbl(mp3_lbl);

    // // Калькулятор
    // MenuItem *calc_item = creator.getMenuItem(ID_CONTEXT_CALC);
    // _menu->addItem(calc_item);

    // Image *calc_img = new Image(1, _display);
    // calc_item->setImg(calc_img);
    // calc_img->setTransparentColor(Image::TRANSPARENT_COLOR);
    // calc_img->init(ICO_WH, ICO_WH);
    // calc_img->setSrc(CALC_IMG);

    // Label *calc_lbl = creator.getItemLabel(STR_CALC_ITEM, 4, 2);
    // calc_item->setLbl(calc_lbl);

    // Читалка
    MenuItem *read_item = creator.getMenuItem(ID_CONTEXT_READER);
    _menu->addItem(read_item);

    Image *read_img = new Image(1, _display);
    read_item->setIco(read_img);
    read_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    read_img->init(ICO_WH, ICO_WH);
    read_img->setSrc(BOOK_IMG);

    Label *read_lbl = creator.getItemLabel(STR_READER_ITEM, 4, 2);
    read_item->setLbl(read_lbl);

    // Ігри
    MenuItem *game_item = creator.getMenuItem(ID_CONTEXT_GAMES);
    _menu->addItem(game_item);

    Image *game_img = new Image(1, _display);
    game_item->setIco(game_img);
    game_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    game_img->init(ICO_WH, ICO_WH);
    game_img->setSrc(JOYSTICK_IMG);

    Label *game_lbl = creator.getItemLabel(STR_GAME_ITEM, 4, 2);
    game_item->setLbl(game_lbl);

    // Файли
    MenuItem *files_item = creator.getMenuItem(ID_CONTEXT_FILES);
    _menu->addItem(files_item);

    Image *files_img = new Image(1, _display);
    files_item->setIco(files_img);
    files_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    files_img->init(ICO_WH, ICO_WH);
    files_img->setSrc(SD_IMG);

    Label *files_lbl = creator.getItemLabel(STR_FILES_ITEM, 4, 2);
    files_item->setLbl(files_lbl);

    // Налаштування
    MenuItem *pref_item = creator.getMenuItem(ID_CONTEXT_PREF_SEL);
    _menu->addItem(pref_item);

    Image *pref_img = new Image(1, _display);
    pref_item->setIco(pref_img);
    pref_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    pref_img->init(ICO_WH, ICO_WH);
    pref_img->setSrc(SETTINGS_IMG);

    Label *pref_lbl = creator.getItemLabel(STR_PREFERENCES_ITEM, 4, 2);
    pref_item->setLbl(pref_lbl);

    // Прошика
    MenuItem *firm_item = creator.getMenuItem(ID_CONTEXT_FIRMWARE);
    _menu->addItem(firm_item);

    Image *firm_img = new Image(1, _display);
    firm_item->setIco(firm_img);
    firm_img->setTransparentColor(Image::COLOR_TRANSPARENT);
    firm_img->init(ICO_WH, ICO_WH);
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

    _menu->setCurrentFocusPos(_last_sel_item_pos);
    _scrollbar->setValue(_last_sel_item_pos);
}

MenuContext::~MenuContext()
{
    for (auto b_it = _bin.begin(), e_it = _bin.end(); b_it != e_it; ++b_it)
        delete *b_it;
}

void MenuContext::loop()
{
}

void MenuContext::update()
{
    if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, HOLD_LOCK);
        up();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
        down();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, CLICK_LOCK);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
         _last_sel_item_pos = 0;
        openContextByID(ID_CONTEXT_HOME);
    }
}

void MenuContext::up()
{
    _menu->focusUp();
    _scrollbar->scrollUp();
}

void MenuContext::down()
{
    _menu->focusDown();
    _scrollbar->scrollDown();
}

void MenuContext::ok()
{
    uint16_t id = _menu->getCurrentItemID();
    _last_sel_item_pos = _menu->getCurrentFocusPos();
    openContextByID((ContextID)id);
}
