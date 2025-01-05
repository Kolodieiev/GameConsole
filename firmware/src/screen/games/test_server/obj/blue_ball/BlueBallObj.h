#pragma once
#include <Arduino.h>
#include "../BallObj.h"

using namespace meow;

namespace test_server
{
    class BlueBallObj : public BallObj
    {
    public:
        BlueBallObj(GraphicsDriver &display,
                    ResManager &res,
                    WavManager &audio,
                    GameMap &game_map,
                    std::unordered_map<uint32_t, IGameObject *> &game_objs) : BallObj(display, res, audio, game_map, game_objs) {}
        virtual ~BlueBallObj() {}

        virtual void init() override;

    private:
    };
}