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
    if(health - amount > 0.f)
        health -= amount;
    else
        health = 0.f;
}

void Player::move(float x, float y)
{
    if(!GameSettings::world.isCollided(getRect().left+x, getRect().top+y, getRect().width, getRect().height))
    {
        position += Vector2f(x,y);
        if(y > 0.f)
            airTime++;
    }
    else //Reset player stat on collide
    {
        velocity = Vector2f(0.f,0.f);
        jumping = false;
        damage((airTime - 10) / 6);
        airTime = 0;
    }
}

FloatRect Player::getRect()
{
    return FloatRect(position.x+0.1f, position.y+0.1f, 0.8f, 2.9f);
}

void Player::update()
{
    move(velocity.x, velocity.y);
    if(health < 0.f)
        respawn();
}

Vector2f Player::getScreenPosition() //inverts Y!
{
    return ScreenSettings::b2PosToScreen(position);
}
