#pragma once

#include "config.hpp"

class Player
{
    Vector2f position;
    bool damagedOnFall;
public:
    bool jumping;
    float health;
    float airTime;
    bool disableVelReset;
    unsigned currentBlock;

    Player();
    Vector2f velocity;
    Vector2f getScreenPosition();
    void setPosition(float x, float y);
    Vector2f getPosition();
    void move(float x, float y, bool disableVelocityResetting = false);
    void damage(float amount);
    void jump();
    void respawn();
    void update();
    FloatRect getRect();
};
