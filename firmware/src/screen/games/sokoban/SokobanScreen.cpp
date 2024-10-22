#include "SokobanScreen.h"
//
#include "../../WidgetCreator.h"
#include "./SceneID.h"
#include "scene/SokobanScene.h"

#include "../../resources/color.h"
#include "../../resources/string.h"
#include "../../resources/const.h"

SokobanScreen::SokobanScreen(GraphicsDriver &display) : IScreen{display}
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    showLvlMenu();
}

void SokobanScreen::loop()
{
}

void SokobanScreen::update()
{
    if (_is_game_started)
    {
        if (!_scene->isFinished())
        {
            if (!_scene->isReleased())
                _scene->update();
            else
            {
                uint8_t next_lvl = _scene->getNextSceneID();
                delete _scene;
                _scene = new SokobanScene(_display, _input, _stored_objs, false, next_lvl);
                _is_game_started = true;
            }
        }
        else
        {
            delete _scene;
            _is_game_started = false;
            showLvlMenu();
        }
    }
    else
    {
        if (_input.isPressed(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, 500);
            openScreenByID(ID_SCREEN_GAMES);
        }
        else if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, 130);
            _lvl_menu->focusUp();
            _scrollbar->scrollUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, 130);
            _lvl_menu->focusDown();
            _scrollbar->scrollDown();
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, 500);
            uint8_t next_lvl = _lvl_menu->getCurrentItemID();
            getLayout()->deleteWidgets();
            _scene = new SokobanScene(_display, _input, _stored_objs, false, next_lvl);
            _is_game_started = true;
        }
    }
}

void SokobanScreen::showLvlMenu()
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = (EmptyLayout *)getLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK));

    _lvl_menu = new FixedMenu(ID_LVL_LIST, _display);
    layout->addWidget(_lvl_menu);
    _lvl_menu->setBackColor(COLOR_MENU_ITEM);
    _lvl_menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _lvl_menu->setHeight(_display.height() - NAVBAR_HEIGHT - 1);
    _lvl_menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / MENU_ITEMS_NUM);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    _scrollbar->setBackColor(COLOR_MAIN_BACK);

    for (uint8_t i{1}; i <= LEVEL_NUM; ++i)
    {
        MenuItem *lvl_item = creator.getMenuItem(i);
        _lvl_menu->addItem(lvl_item);

        String lbl = STR_LVL;
        lbl += i;

        Label *lvl_lbl = creator.getItemLabel(lbl.c_str());
        lvl_item->setLbl(lvl_lbl);
    }

    _scrollbar->setMax(_lvl_menu->getSize());
}
