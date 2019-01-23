#include "Inventory.h"
#include "Settings.h"

void Inventory::draw()
{
    for(int i = 0; i < sizeX; i++)
    for(int j = 0; j < sizeY; j++)
    {
        Vector2f mousePos(ScreenSettings::window.mapPixelToCoords(Mouse::getPosition(ScreenSettings::window)));
        Vector2f slotPos = getSlotPos(i, j);
        Vector2i pointedSlot = getSlotByPos(mousePos.x, mousePos.y);
        bool hover = pointedSlot.x == i && pointedSlot.y == j;

        Color bgColor = j != 0 ? Color(128, 128, 128, 255) : Color(255, 64, 64, 255);
        // Background
        if(hover)
            bgColor -= Color(64, 64, 64, 0);

        RectangleShape bgRS(Vector2f(50.f, 50.f));
        bgRS.setFillColor(bgColor);
        bgRS.setOrigin(25.f, 25.f);
        bgRS.setPosition(slotPos);
        ScreenSettings::window.draw(bgRS);

        ScreenRenderer::drawItem(getItem(i,j), slotPos);
    }
}

Inventory::Inventory(Vector2f pos, int x, int y) : sizeX(x), sizeY(y), invPos(pos)
{
    items.resize(x*y); //Allocate item array
}
