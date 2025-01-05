#pragma once
#include <Arduino.h>
#include "meow/game/object/IGameObject.h"

using namespace meow;

namespace sokoban
{
    class BoxPointObj : public IGameObject
    {
    public:
        BoxPointObj(GraphicsDriver &display,
                    ResManager &res,
                    WavManager &audio,
                    GameMap &game_map,
                    std::unordered_map<uint32_t, IGameObject *> &game_objs) : IGameObject(display, res, audio, game_map, game_objs) {}
        virtual ~BoxPointObj() {}

        virtual void init() override;
        virtual void update() override;
        virtual void serialize(DataStream &ds) override;
        virtual void deserialize(DataStream &ds) override;
        virtual size_t getDataSize() const override;
    };
}