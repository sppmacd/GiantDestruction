#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player()
{
    position = Vector2f(5.f, 0.f);
    jumping = false;
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
    return FloatRect(position.x, position.y, 1.f, 3.f);
}

void Player::update()
{
    move(velocity.x, velocity.y);
}

Vector2f Player::getScreenPosition() //inverts Y!
{
    return ScreenSettings::b2PosToScreen(position);
}
