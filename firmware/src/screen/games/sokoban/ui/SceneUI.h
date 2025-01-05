#pragma once
#include <Arduino.h>

#include "meow/game/IGameUI.h"
#include "meow/game/object/IGameObject.h"

using namespace meow;

namespace sokoban
{
    class SceneUI : public IGameUI
    {
    public:
        SceneUI(GraphicsDriver &display) : IGameUI(display) {}
        virtual void onDraw() override;

    private:
    };
}