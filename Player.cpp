#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player()
: Entity(Vector2f(5.f, 15.f)), inventory(Inventory(Vector2f(400.f, 400.f)))
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
        item.count = 1;
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

void Player::inventoryOnSplit(Vector2i mousePos)
{
    Vector2i slot = inventory.getSlotByPos(mousePos.x, mousePos.y);
    if(!GameSettings::currentSplittedItemSlots.empty() && GameSettings::currentSplittedItemSlots.back() != slot)
    {
        if(inventory.getItem(slot.x, slot.y).id == ItemType::ITEMTYPE_AIR || (inventory.getItem(slot.x, slot.y).id == GameSettings::currentPickedItem.id && inventory.getItem(slot.x, slot.y).count + GameSettings::currentPickedItem.count <= 32))
        {
            GameSettings::currentSplittedItemSlots.push_back(slot);
            GameSettings::splitting = true;
        }
    }
}

void Player::inventoryOnRightClick(Vector2i mousePos)
{
    Vector2i slot = inventory.getSlotByPos(mousePos.x, mousePos.y);
    Item item = inventory.getItem(slot.x, slot.y);

    if(item.count + 1 <= 32 && GameSettings::currentPickedItem.count - 1 >=0 && ((item.id == GameSettings::currentPickedItem.id && item.damage == GameSettings::currentPickedItem.damage) || item.id == 0))
    {
        inventory.setItem(slot.x, slot.y, Item(GameSettings::currentPickedItem.id, GameSettings::currentPickedItem.damage, ++item.count));
        GameSettings::currentPickedItem.count--;
    }
}

void Player::inventoryOnDoubleClick(Vector2i mousePos)
{
    Vector2i slot = inventory.getSlotByPos(mousePos.x, mousePos.y);
    inventory.onDoubleClickSlot(slot.x, slot.y);
}

void Player::inventoryOnSplitFinished()
{
    int slotSize = GameSettings::currentPickedItem.count / GameSettings::currentSplittedItemSlots.size();

    for(Vector2i slot: GameSettings::currentSplittedItemSlots)
    {
        inventory.setItem(slot.x, slot.y, Item(GameSettings::currentPickedItem.id, GameSettings::currentPickedItem.damage, slotSize));
    }

    GameSettings::currentPickedItem = Item(GameSettings::currentPickedItem.id, GameSettings::currentPickedItem.damage, GameSettings::currentPickedItem.count - slotSize * GameSettings::currentSplittedItemSlots.size());
}
