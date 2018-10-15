#pragma once

#include "config.hpp"

class Player
{
    Vector2f position;
    Vector2f velocity;
    bool moved;
public:
    Player();
    Vector2f getScreenPosition();
    void move(float x, float y);
    void update();
};
