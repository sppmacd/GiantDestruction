#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player()
: Entity(Vector2f(5.f, 15.f))
{
    respawn();
    jumpForce = 0.2f;
    type = Entity::ENTITY_PLAYER;
}

void Player::respawn()
{
    position = Vector2f(5.f, 15.f);
    health = 120.f;
    currentBlock = 0;
    jumping = false;
    airTime = 0;
    velocity = Vector2f(0.f, 0.f);
    damagedOnFall = true; //to force not damaging on spawn

    inventory.clear();

    for(int i = 1; i < 16; i++)
    {
        Item item;
        item.id = i;
        inventory.setItem((i-1) % 9, (i-1) / 9, item);
    }
}

FloatRect Player::getRect()
{
    return FloatRect(position.x-0.4f, position.y-2.9f, 0.8f, 2.9f);
}

void Player::update()
{
    Entity::update();
    if(health <= 0.f)
        respawn();
}

void Player::inventoryOnClick(Vector2i mousePos)
{
    Vector2i slot = inventory.getSlotByPos(mousePos.x, mousePos.y);
    if(GameSettings::currentPickedItem.id == 0 && inventory.getItem(slot.x, slot.y).id != 0)
        inventory.onPick(slot.x, slot.y);
    else
        inventory.onPut(slot.x, slot.y);
}
