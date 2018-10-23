#include "Entity.h"
#include "Settings.h"
#include "WorldBlockSettings.h"

Entity::Entity(Vector2f startPos)
{
    position = startPos;
    jumping = false;
    airTime = 0;
    velocity = Vector2f(0.f, 0.f);
    damagedOnFall = true;
    jumpForce = 0.12f;
    type = Entity::ENTITY_NORMAL;
    shown = true;
}

Entity::~Entity()
{
    //dtor
}

void Entity::update()
{
    move(velocity.x, velocity.y);

    //// AI /////

    if(type == Entity::ENTITY_NORMAL)
    {
        timer++;

        if(jumping == false)
            velocity.x = 0.f;

        if(shown)
        {
            Entity::CollisionType type;
            // Handle moving (walking)
            if(dir)
                type = move(-0.05f * (speeding ? 2.f : 1.f), 0.f);
            else
                type = move(0.05f * (speeding ? 2.f : 1.f), 0.f);

            // Handle collision and jumping
            switch(type)
            {
            case Entity::COLLTYPE_UP:
                velocity.y = 0.f;
                break;
            case Entity::COLLTYPE_DOWN:
                velocity.y = 0.f;
                break;
            case Entity::COLLTYPE_RIGHT:
                move(-0.05f * (speeding ? 2.f : 1.f), 0.f);
                velocity.x = 0.05f;
                jump();
                break;
            case Entity::COLLTYPE_LEFT:
                move(0.05f * (speeding ? 2.f : 1.f), 0.f);
                velocity.x = -0.05f;
                jump();
                break;
            default:
                break;
            }
        }

        if(timer > 10)
        {
            if(shown && GameSettings::world.getBlock(position.x, position.y).blockType == BlockType::TYPE_DOORS)
            {
                shown = false;
                timer = 0;
            }
            // Handle RBB
            if(timer > 50)
            {
                if(!shown && !(rand() % 2))
                {
                    shown = true;
                    timer = 0;
                }
                if(speeding)
                    speeding = false;
            }
        }

        // Handle direction changing
        if(!(rand() % 100))
            dir = !dir;

        // Handle speeding from giant
        float distGiant = GameSettings::world.getPlayer().getPosition().x - position.x;
        if(distGiant > -3.f && distGiant < 0.f)
        {
            dir = false;
            speeding = true;
            shown = true;
            timer = 0;
        }
        else if(distGiant < 3.f && distGiant > 0.f)
        {
            dir = true;
            speeding = true;
            shown = true;
            timer = 0;
        }

        if(position.x < 1.f)
            dir = false;
        if(position.x > GameSettings::WORLD_SIZE_X - 1.f)
            dir = true;
    }
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

Entity::CollisionType Entity::move(float x, float y, bool disableVelocityResetting)
{
    if(disableVelocityResetting)
        disableVelReset = true;

    bool collide = false;
    if(x == 0 && y == 0)
        return Entity::COLLTYPE_INVALID;

    if(!GameSettings::world.isCollided(getRect().left+x, getRect().top+y, getRect().width, getRect().height, type == Entity::ENTITY_NORMAL))
    {
        position += Vector2f(x,y);
        if(y > 0.f)
            airTime += y * 0.2f / jumpForce;
    }
    else //Reset player stat on collide
    {
        collide = true;
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

    // check collision type if collided
    if(collide)
    {
        if(y < 0 && x == 0) return Entity::COLLTYPE_UP;
        else if(y > 0 && x == 0) return Entity::COLLTYPE_DOWN;
        else if(y == 0 && x > 0) return Entity::COLLTYPE_RIGHT;
        else if(y == 0 && x < 0) return Entity::COLLTYPE_LEFT;
        else if(y == 0 && x < 0) return Entity::COLLTYPE_LEFT;
        else if(y < 0 && x > 0) return Entity::COLLTYPE_UP_RIGHT;
        else if(y < 0 && x < 0) return Entity::COLLTYPE_UP_LEFT;
        else if(y > 0 && x > 0) return Entity::COLLTYPE_DOWN_RIGHT;
        else if(y > 0 && x < 0) return Entity::COLLTYPE_DOWN_LEFT;
    }
    return Entity::COLLTYPE_NONE;
}

FloatRect Entity::getRect()
{
    return FloatRect(position.x-0.1f, position.y-0.4f, 0.2f, 0.4f);
}
