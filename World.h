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

class Chunk;

class World
{
    Player player;
public:
    World();
    void init();
    virtual ~World();
    struct Block
    {
        unsigned heightType : 4;
        unsigned blockType : 4;
        unsigned meta : 4;
        unsigned flags : 4;

        bool hasFlag(unsigned flag);
        void setFlag(unsigned flag);
        void unsetFlag(unsigned flag);
        FloatRect getRect(int x, int y);
        Block(unsigned short code);
        Block() : Block(1) {}
    };

    bool isCollided(float x, float y, float sx, float sy);
    bool isCollidedWithPlayer(int x, int y);
    void update();
    void movePlayer(float x, float y, bool disableReset = false);
    void acceleratePlayer(float x, float y);
    void placeBlock(int x, int y);
    Player& getPlayer();
    void jump();
    void loadFromFile(int chunkId);
    void respawnPlayer(float x, float y);
    void setBlock(int x, int y, Block& block);
    Block getBlock(int x, int y);
    void draw(RenderWindow& wnd);
    void addChunk(Chunk* chunk, int id);
private:
    map<int,Chunk*> chunks;
};

class Chunk
{
    World::Block blocks[9][18];
public:
    Chunk();
    void setBlock(int x, int y, World::Block& block);
    World::Block getBlock(int x, int y);
};
