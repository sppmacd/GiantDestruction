#pragma once

#include "config.hpp"

class Player
{
    b2Body* b2Handler;
public:
    Player() {}
    Player(b2World* world);
    Vector2f getScreenPosition();
};
