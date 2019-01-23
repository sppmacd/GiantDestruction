#pragma once

#include "Inventory.h"
#include "Entity.h"

class Player : public Entity
{
public:
    unsigned currentBlock;

    Inventory inventory;

    Player();
    void inventoryOnClick(Vector2i mousePos);
    void respawn();
    void update();
    FloatRect getRect();
};
