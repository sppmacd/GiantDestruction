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
}

Vector2f Player::getPosition()
{
    return position;
}

void Player::setPosition(float x, float y)
{
    position = Vector2f(x,y);
}

void Player::move(float x, float y)
{
    if(!GameSettings::world.isCollided(getRect().left+x, getRect().top+y, getRect().width, getRect().height))
        position += Vector2f(x,y);
    else //Reset player stat on collide
    {
        velocity = Vector2f(0.f,0.f);
        jumping = false;
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
