#pragma once

#include "config.hpp"

class Player
{
    Vector2f position;
    bool moved;
public:
    bool jumping;
    float health;

    Player();
    Vector2f velocity;
    Vector2f getScreenPosition();
    void setPosition(float x, float y);
    Vector2f getPosition();
    void move(float x, float y);
    void respawn();
    void update();
    FloatRect getRect();
};
