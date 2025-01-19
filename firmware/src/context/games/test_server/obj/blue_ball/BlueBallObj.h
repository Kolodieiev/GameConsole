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
                    TerrainManager &terrain,
                    std::unordered_map<uint32_t, IGameObject *> &game_objs) : BallObj(display, res, audio, terrain, game_objs) {}
        virtual ~BlueBallObj() {}

        virtual void init() override;

    private:
    };
}