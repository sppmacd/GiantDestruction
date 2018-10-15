#include "Settings.h"

Vector2f ScreenSettings::b2PosToScreen(Vector2f vec)
{
    float x = ScreenSettings::getBlockSize()*vec.x;
    float y = ScreenSettings::getBlockSize()*vec.y;
    return Vector2f(x,y);
}

float ScreenSettings::getBlockSize()
{
    return 32.f;
}

void ScreenRenderer::drawWorld()
{
    GameSettings::world.draw(ScreenSettings::window);
}
