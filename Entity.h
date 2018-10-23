#pragma once

#include "config.hpp"

class Entity
{
public:
    Vector2f velocity;
    bool jumping;
    float health;
    float airTime;
    bool disableVelReset;
    float jumpForce;

    Entity(Vector2f startPos);
    virtual ~Entity();

    void jump();

    virtual void update();

    void setPosition(float x, float y);

    Vector2f getPosition();

    void move(float x, float y, bool disableVelocityResetting = false);

    void damage(float amount);

    Vector2f getScreenPosition();

    virtual FloatRect getRect();
protected:

    Vector2f position;

    bool damagedOnFall;
private:

};
