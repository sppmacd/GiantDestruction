#pragma once

#include "config.hpp"
#include "Player.h"

namespace BlockFlags
{
    extern const int WORLD_BACK_LAYER;
    extern const int WORLD_HARVESTABLE;
    extern const int WORLD_UPDATEABLE;
    extern const int WORLD_PHYSICAL;
}

class World
{
    Player player;
public:
    World();
    struct Block
    {
        int heightType : 4;
        int blockType : 4;
        int meta : 4;
        int flags : 4;

        bool hasFlag(int flag);
        void setFlag(int flag);
        void unsetFlag(int flag);
        FloatRect getRect(int x, int y);
    };

    bool isCollided(float x, float y, float sx, float sy);
    void update();
    void movePlayer(float x, float y);
    void acceleratePlayer(float x, float y);
    void jump();
    void setBlock(int x, int y, Block& block);
    Block getBlock(int x, int y);
    void draw(RenderWindow& wnd);
private:
    Block blocks[48][32];
};