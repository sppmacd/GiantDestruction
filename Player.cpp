#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player()
{
    respawn();
}

void Player::respawn()
{
    position = Vector2f(5.f, 0.f);
    jumping = false;
    health = 120.f;
    airTime = 0;
    velocity = Vector2f(0.f, 0.f);
    damagedOnFall = false;
}

Vector2f Player::getPosition()
{
    return position;
}

void Player::setPosition(float x, float y)
{
    position = Vector2f(x,y);
}

void Player::damage(float amount)
{
    if(amount > 0.f && health - amount > 0.f)
        health -= amount;
    else
        health = 0.f;
}

void Player::move(float x, float y, bool disableVelocityResetting)
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
        if(!disableVelReset)
        {
            velocity = Vector2f(0.f,0.f);
        }
        else
        {
            disableVelReset = false;
        }

        if(!damagedOnFall)
        {
            velocity = Vector2f(0.f,0.f);
            if(y > 0.f)
            {
                jumping = false;
                if((airTime - 3) > 0.f)
                    damage((airTime - 3));
                airTime = 0;
                damagedOnFall = true;
            }
        }
    }
}

FloatRect Player::getRect()
{
    return FloatRect(position.x+0.1f, position.y+0.1f, 0.8f, 2.9f);
}

void Player::jump()
{
    if(!jumping)
    {
        velocity += Vector2f(0.f, -0.2f);
        jumping = true;
        damagedOnFall = false;
    }
}

void Player::update()
{
    move(velocity.x, velocity.y);
    if(health <= 0.f)
        respawn();

    if(GameSettings::world.isCollided(getRect().left, getRect().top - 1.f, getRect().width, getRect().height))
        velocity = Vector2f(0.f,0.f);
}

Vector2f Player::getScreenPosition() //inverts Y!
{
    return ScreenSettings::b2PosToScreen(position);
}
