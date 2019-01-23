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
    void inventoryOnSplit(Vector2i mousePos);
    void inventoryOnSplitFinished();
    void inventoryOnRightClick(Vector2i mousePos);
    void inventoryOnDoubleClick(Vector2i mousePos);
    void respawn();
    void update();
    FloatRect getRect();
};
