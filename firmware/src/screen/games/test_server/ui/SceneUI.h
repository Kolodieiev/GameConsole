#pragma once
#include <Arduino.h>

#include "meow/game/IGameUI.h"
#include "meow/game/object/IGameObject.h"

using namespace meow;

namespace test_server
{
    class SceneUI : public IGameUI
    {
    public:
        SceneUI(GraphicsDriver &display);
        virtual ~SceneUI() {}
        virtual void onDraw() override;
        void hideStartMsg();

    private:
        uint16_t COLOR_TRANSPARENT{0xF81F};
        bool _is_msg_hide{false};
    };
}