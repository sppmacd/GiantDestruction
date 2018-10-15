#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player()
{
    position = Vector2f(0.f, 0.f);
}

void Player::move(float x, float y)
{
    position += Vector2f(x,y);
}

void Player::update()
{
    if(moved)
    {
        //b2Handler->ApplyLinearVelocity(b2Vec2(0.f,0.f));
        moved = false;
    }
}

Vector2f Player::getScreenPosition() //inverts Y!
{
    return ScreenSettings::b2PosToScreen(position);
}
