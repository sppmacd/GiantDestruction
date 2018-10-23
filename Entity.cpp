#include "Entity.h"
#include "Settings.h"

Entity::Entity(Vector2f startPos)
{
    position = startPos;
    jumping = false;
    airTime = 0;
    velocity = Vector2f(0.f, 0.f);
    damagedOnFall = false;
    jumpForce = 0.12f;
}

Entity::~Entity()
{
    //dtor
}

void Entity::update()
{
    move(velocity.x, velocity.y);
}

Vector2f Entity::getScreenPosition()
{
    return ScreenSettings::b2PosToScreen(position);
}


Vector2f Entity::getPosition()
{
    return position;
}

void Entity::jump()
{
    if(!jumping)
    {
        move(0.f, -0.1f);
        velocity += Vector2f(0.f, -jumpForce);
        jumping = true;
        damagedOnFall = false;
    }
}

void Entity::setPosition(float x, float y)
{
    position = Vector2f(x,y);
}

void Entity::damage(float amount)
{
    if(amount > 0.f && health - amount > 0.f)
        health -= amount;
    else
        health = 0.f;
}

void Entity::move(float x, float y, bool disableVelocityResetting)
{
    if(disableVelocityResetting)
        disableVelReset = true;

    if(!GameSettings::world.isCollided(getRect().left+x, getRect().top+y, getRect().width, getRect().height))
    {
        position += Vector2f(x,y);
        if(y > 0.f)
            airTime += y;
    }
    else //Reset player stat on collide
    {
        if(y != 0.f)
        {
            jumping = false;
            velocity.y = 0.f;
        }
        if(x != 0.f) velocity.x = 0.f;

        if(y > 0.f)
        {
            if(!damagedOnFall)
            {
                if((airTime - 3) > 0.f)
                    damage((airTime - 3));
                airTime = 0;
                damagedOnFall = true;
            }
        }
    }
}

FloatRect Entity::getRect()
{
    return FloatRect(position.x-0.1f, position.y-0.4f, 0.2f, 0.4f);
}
