#pragma once
#include <Arduino.h>
//
#include "meow/ui/screen/IScreen.h"
#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"
#include "meow/game/IGameScene.h"

using namespace meow;

namespace sokoban
{
    class SokobanScreen : public IScreen
    {

    public:
        SokobanScreen(GraphicsDriver &display);
        virtual ~SokobanScreen() {}

    protected:
        virtual void loop() override;
        virtual void update() override;

    private:
        const uint8_t MENU_ITEMS_NUM{10};
        const uint8_t LEVEL_NUM{70};

        enum Widget_ID : uint8_t
        {
            ID_NAVBAR = 1,
            ID_LVL_LIST,
            ID_SCROLL
        };

        bool _is_game_started{false};

        FixedMenu *_lvl_menu;
        ScrollBar *_scrollbar;

        IGameScene *_scene;
        std::vector<IObjShape *> _stored_objs;

        void showLvlMenu();
    };
}