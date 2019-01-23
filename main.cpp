#include "config.hpp"

#include "World.h"
#include "Settings.h"

using namespace std;

namespace ScreenSettings
{
const float X_BY_Y = 16.f/9.f;
Vector2u windowSize;
View currentWorldView;
View currentInventoryView;
RenderWindow window;
}

namespace GameSettings
{
World world;
bool loaded = false;
Item currentPickedItem = 0;
bool inventoryOpened = false;
}

int main()
{
    ScreenSettings::window.create(sf::VideoMode(ScreenSettings::X_BY_Y*800, 800), "Giant Destruction", Style::Default ^ Style::Resize);
    ScreenSettings::window.setView(ScreenSettings::currentWorldView);
    ScreenSettings::window.setVerticalSyncEnabled(true);

    ScreenRenderer::drawLoadingProgress("Loading game", "Loading renderer");

    ScreenSettings::currentWorldView = ScreenSettings::window.getDefaultView();
    ScreenSettings::currentGUIView = ScreenSettings::window.getDefaultView();
    ScreenSettings::currentInventoryView = ScreenSettings::window.getDefaultView();
    ScreenSettings::loadTextures();

    GameSettings::world.init();
    GameSettings::loaded = true;

    Clock lastClickCounter;

    while (ScreenSettings::window.isOpen())
    {
        ScreenSettings::window.setView(ScreenSettings::currentWorldView);

        Event event;
        while (ScreenSettings::window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                ScreenSettings::window.close();
            if (event.type == Event::MouseButtonPressed)            // item drag and drop.
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    if(GameSettings::inventoryOpened)
                    {
                        if(lastClickCounter.getElapsedTime().asMilliseconds() > 200)
                        {
                            GameSettings::currentSplittedItemSlots.push_back(GameSettings::world.getPlayer().inventory.getSlotByPos(event.mouseButton.x, event.mouseButton.y));
                        }
                        else
                        {
                            GameSettings::world.getPlayer().inventoryOnDoubleClick(Vector2i(event.mouseButton.x, event.mouseButton.y));
                        }
                    }
                    else
                    {
                        Vector2f pos = ScreenSettings::screenPosToB2(ScreenSettings::window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y)));
                        GameSettings::world.placeBlock(pos.x, pos.y);
                    }

                    lastClickCounter.restart();
                }
                else if(event.mouseButton.button == Mouse::Right)
                {
                    GameSettings::world.getPlayer().inventoryOnRightClick(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            if (event.type == Event::KeyPressed && event.key.code >= 27 && event.key.code <= 35)
            {
                int number = event.key.code - 27;
                GameSettings::world.getPlayer().currentBlock = number;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::E)
            {
                GameSettings::inventoryOpened = !GameSettings::inventoryOpened;
            }
            if (event.type == Event::MouseMoved)
            {
                if (Mouse::isButtonPressed(Mouse::Left) && GameSettings::inventoryOpened && GameSettings::currentPickedItem.id != ItemType::ITEMTYPE_AIR)
                {
                    GameSettings::world.getPlayer().inventoryOnSplit(Vector2i(event.mouseMove.x, event.mouseMove.y));
                }
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                if(GameSettings::inventoryOpened)
                {
                    if(GameSettings::splitting)
                    {
                        GameSettings::splitting = false;
                        GameSettings::world.getPlayer().inventoryOnSplitFinished();
                    }
                    else
                    {
                        GameSettings::world.getPlayer().inventoryOnClick(Vector2i(event.mouseButton.x, event.mouseButton.y));
                    }
                    GameSettings::currentSplittedItemSlots.clear();
                }
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::D)) GameSettings::world.movePlayer(0.2f, 0.f, true);
        if(Keyboard::isKeyPressed(Keyboard::A)) GameSettings::world.movePlayer(-0.2f, 0.f, true);

        if(Keyboard::isKeyPressed(Keyboard::W)) GameSettings::world.jump();
        // shovel - right button

        if(Keyboard::isKeyPressed(Keyboard::S)) GameSettings::world.movePlayer(0.f, 0.2f, true);

        // UPDATE GAME LOGIC
        GameSettings::world.update();
        if(GameSettings::currentPickedItem.count == 0)
            GameSettings::currentPickedItem = 0;

        // RENDER GAME
        ScreenSettings::window.clear();

        ScreenSettings::windowSize = Vector2u(ScreenSettings::X_BY_Y*800, 800);

        ScreenRenderer::drawWorld();
        ScreenSettings::window.setView(ScreenSettings::currentGUIView);
        ScreenRenderer::drawGUI();

        ScreenSettings::window.display();
    }

    return 0;
}
