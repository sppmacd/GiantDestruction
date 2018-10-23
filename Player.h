#pragma once

#include "Inventory.h"
#include "Entity.h"

class Player : public Entity
{
public:
    unsigned currentBlock;

    Inventory<9,3> inventory;

    Player();
    void inventoryOnClick(Vector2i mousePos);
    void respawn();
    void update();
    FloatRect getRect();
};
