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
    int meta : 8;
    int count : 8; //Item amount
    int reserved : 8; //reserved data
    Item(int code)
    {
        id = (code & 0xFF000000) >> 24;
        meta = (code & 0xFF0000) >> 16;
        count = (code & 0xFF00) >> 8;
        reserved = code & 0xFF;
    }

    Item() : Item(0) {};
};

namespace GameSettings
{
extern Item currentPickedItem;
}

template<int sizeX = 9, int sizeY = 3>
class Inventory
{
public:
    Vector2i getSlotByPos(float screenX, float screenY)
    {
        return Vector2i((screenX-400.f)/50.f, (screenY-400.f)/50.f);
    }

    Vector2f getSlotPos(int slotX, int slotY)
    {
        return Vector2f(slotX*50+400, slotY*50+400);
    }

    Vector2i getSize()
    {
        return Vector2i(sizeX, sizeY);
    }

    Item getItem(int slotX, int slotY)
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
            return items[slotX][slotY];
        else
            return Item(0);
    }

    void setItem(int slotX, int slotY, Item item)
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
            items[slotX][slotY] = item;
    }

    void onPick(int slotX, int slotY) //on player clicked the left mouse button and the item haven't been set
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
        {

            GameSettings::currentPickedItem = getItem(slotX, slotY);
            setItem(slotX, slotY, 0);
        }
    }

    void onPut(int slotX, int slotY)  //on player clicked the left mouse button and the item have been set
    {
        if(slotX >= 0 && slotX < sizeX && slotY >= 0 && slotY < sizeY)
        {
            auto item = getItem(slotX, slotY);
            auto it2 = GameSettings::currentPickedItem;
            setItem(slotX, slotY, it2);
            GameSettings::currentPickedItem = item;
        }
    }

    void clear()
    {
        for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            setItem(i,j,0);
    }

private:
    Item items[sizeX][sizeY];
};

#include "Inventory.inl"
