#pragma once

#include "config.hpp"

class Player
{
    b2Body* b2Handler;
public:
    Player(b2Body* handler);
    Vector2f getScreenPosition();
};
