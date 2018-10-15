#pragma once

#include "config.hpp"
#include "World.h"

namespace ScreenSettings
{
extern const float X_BY_Y;
extern Vector2u windowSize;
extern View currentWorldView;
extern RenderWindow window;

Vector2f b2PosToScreen(b2Vec2 vec);
float getBlockSize();
}

namespace GameSettings
{
extern World world;
extern const int WORLD_SIZE_X;
extern const int WORLD_SIZE_Y;
}

namespace ScreenRenderer
{
void drawWorld();
}
