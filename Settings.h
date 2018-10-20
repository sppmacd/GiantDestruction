#pragma once

#include "config.hpp"
#include "World.h"

namespace ScreenSettings
{
extern const float X_BY_Y;
extern Vector2u windowSize;
extern View currentWorldView;
extern View currentGUIView;
extern RenderWindow window;
extern map<string,Texture> textures;
extern float zoom;
extern Font font;

Vector2f b2PosToScreen(Vector2f vec);
Vector2f screenPosToB2(Vector2f vec);
float getBlockSize();
void loadTextures();
void loadTexture(string name);
Texture& getTexture(string name);
void makeZoom(float zoom);
}

namespace GameSettings
{
extern World world;
extern const int WORLD_SIZE_X;
extern const int WORLD_SIZE_Y;
extern bool loaded;

void saveDefaultWorld();
}

//extern const unsigned short DEFAULT_WORLD[][9][18];

namespace ScreenRenderer
{
void drawWorld();
void drawBlock(RenderWindow& wnd, World::Block block, int x, int y);
void drawGUI();
void drawLoadingProgress(string header, string text);
}
