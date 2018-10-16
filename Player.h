#pragma once

#include "config.hpp"

class Player
{
    Vector2f position;
    bool moved;
public:
    bool jumping;
    Player();
    Vector2f velocity;
    Vector2f getScreenPosition();
    void move(float x, float y);
    void update();
    FloatRect getRect();
};
