#pragma once

//create time: 13:29

#include "config.hpp"

namespace ItemType
{
    enum
    {
        //block types
        ITEMTYPE_AIR,
        ITEMTYPE_STONE,
        ITEMTYPE_DIRT,
        ITEMTYPE_GRASS,
        ITEMTYPE_BRICK,
        ITEMTYPE_STONEBRICK,
        ITEMTYPE_GLASS,
        ITEMTYPE_PLANKS,
        ITEMTYPE_WOOD,
        ITEMTYPE_LEAVES,
        ITEMTYPE_DOORS,
        ITEMTYPE_WINDOWS,
        ITEMTYPE_RESERVED_C,
        ITEMTYPE_RESERVED_D,
        ITEMTYPE_RESERVED_E,
        ITEMTYPE_RESERVED_F,
        ITEMTYPE_UNDEFINED //keep last
    };
}

struct Item
{
    int id : 8; //The ID of the item
    int damage : 16;
    int count : 8; //Item amount

    Item(int code)
    {
        id = (code & 0xFF000000) >> 24;
        damage = (code & 0xFFFF00) >> 8;
        count = (code & 0xFF);
    }

    Item(int _id, int _dmg, int _c)
    {
        id = _id;
        damage = _dmg;
        count = _c;
    }

    Item() : Item(0) {};
};

namespace GameSettings
{
extern Item currentPickedItem;
}

class Inventory
{
public:
    const int sizeX, sizeY;
    Vector2f invPos;

    Inventory(Vector2f pos, int x = 9, int y = 3);

    Vector2i getSlotByPos(float screenX, float screenY)
    {
        return Vector2i((screenX-invPos.x+25.f)/50.f, (screenY-invPos.y+25.f)/50.f);
    }

    Vector2f getSlotPos(int slotX, int slotY)
    {
        return Vector2f(slotX*50, slotY*50) + invPos;
    }

    Vector2i getSize()
    {
        return Vector2i(sizeX, sizeY);
    }

    Item getItem(int slotX, int slotY)
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
            return items[slotX * sizeY + slotY];
        else
            return Item(0);
    }

    void setItem(int slotX, int slotY, Item item)
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
            items[slotX * sizeY + slotY] = item;
    }

    void addItem(Item item)
    {
        for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
        {
            Item item2 = getItem(i,j);
            if(item2.id == item.id && item2.damage == item.damage)
            {
                if(item2.count + item.count <= 32)
                {
                    Item item3;
                    item3.id = item2.id;
                    item3.count = item2.count + item.count;
                    //item3.damage = item2.damage;
                    setItem(i,j,item3);
                    return;
                }
            }
            else if(item2.id == ItemType::ITEMTYPE_AIR)
            {
                setItem(i,j,item);
                return;
            }
        }

        // TODO cannot add item - spawn item entity.
    }

    void onPick(int slotX, int slotY) //on player clicked the left mouse button and the item hasn't been set
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
        {
            GameSettings::currentPickedItem = getItem(slotX, slotY);
            setItem(slotX, slotY, 0);
        }
    }

    void onPut(int slotX, int slotY)  //on player clicked the left mouse button and the item has been set
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
        {
            Item item = getItem(slotX, slotY);
            Item it2 = GameSettings::currentPickedItem;

            if(item.id == it2.id && item.damage == it2.damage)
            {
                if(item.count + it2.count <= 32)
                {
                    Item item2;
                    item2.count = item.count + it2.count;
                    item2.id = item.id;
                    item2.damage = item.damage;
                    setItem(slotX, slotY, item2);
                    GameSettings::currentPickedItem = Item();
                }
                else
                {
                    Item item2;
                    item2.count = 32;
                    item2.id = item.id;
                    item2.damage = item.damage;
                    setItem(slotX, slotY, item2);

                    item2.count = item.count + it2.count - 32;
                    GameSettings::currentPickedItem = item2;
                }
            }
            else
            {
                setItem(slotX, slotY, it2);
                GameSettings::currentPickedItem = item;
            }
        }
    }

    void clear()
    {
        for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            setItem(i,j,0);
    }

    /// Draws the inventory to window
    void draw();

private:
    vector<Item> items;
};

#include "Inventory.inl"
