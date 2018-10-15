#include "Settings.h"

Vector2f ScreenSettings::b2PosToScreen(b2Vec2 vec)
{
    float x = ScreenSettings::windowSize.x/(X_BY_Y*9.f)*vec.x;
    float y = ScreenSettings::windowSize.y/9.f*vec.y;
    return Vector2f(x,y);
}

float ScreenSettings::getBlockSize()
{
    return ScreenSettings::windowSize.y / 9.f;
}

void ScreenRenderer::drawWorld()
{
    GameSettings::world.draw(ScreenSettings::window);
}
