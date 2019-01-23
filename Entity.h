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
    bool shown;
    int hurtTime;

    enum Type
    {
        ENTITY_NORMAL,
        ENTITY_PLAYER,
        ENTITY_UNDEFINED
    };

    enum CollisionType
    {
        COLLTYPE_INVALID = -1,
        COLLTYPE_NONE = 0,
        COLLTYPE_UP,
        COLLTYPE_DOWN,
        COLLTYPE_RIGHT,
        COLLTYPE_LEFT,
        COLLTYPE_UP_RIGHT,
        COLLTYPE_UP_LEFT,
        COLLTYPE_DOWN_RIGHT,
        COLLTYPE_DOWN_LEFT
    };

    Type type;

    Entity(Vector2f startPos);
    virtual ~Entity();

    void jump();

    virtual void update();

    void setPosition(float x, float y);

    Vector2f getPosition();

    CollisionType move(float x, float y, bool disableVelocityResetting = false);

    void damage(float amount);

    Vector2f getScreenPosition();

    virtual FloatRect getRect();
protected:

    Vector2f position;

    bool damagedOnFall;
private:

    //// AI ////

    bool dir; //true if left
    int timer;
    bool speeding; //true if the entity is running away from giant

};
