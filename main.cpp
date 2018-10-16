#include "config.hpp"

#include "Player.h"
#include "World.h"
#include "Settings.h"

using namespace std;

namespace ScreenSettings
{
const float X_BY_Y = 16.f/9.f;
Vector2u windowSize;
View currentWorldView;
RenderWindow window;
}

namespace GameSettings
{
World world;
}

int main()
{
    ScreenSettings::window.create(sf::VideoMode(ScreenSettings::X_BY_Y*800, 800), "Giant Destruction", Style::Default ^ Style::Resize);
    ScreenSettings::window.setView(ScreenSettings::currentWorldView);
    ScreenSettings::window.setVerticalSyncEnabled(true);

    ScreenSettings::currentWorldView = ScreenSettings::window.getDefaultView();
    ScreenSettings::currentGUIView = ScreenSettings::window.getDefaultView();
    ScreenSettings::loadTextures();
    ScreenSettings::zoom = 1.f;

    while (ScreenSettings::window.isOpen())
    {
        ScreenSettings::window.setView(ScreenSettings::currentWorldView);

        Event event;
        while (ScreenSettings::window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                ScreenSettings::window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2f pos = ScreenSettings::screenPosToB2(ScreenSettings::window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y)));
                GameSettings::world.placeBlock(pos.x, pos.y);
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::W)) GameSettings::world.jump();

        if(Keyboard::isKeyPressed(Keyboard::D)) GameSettings::world.movePlayer(0.2f, 0.f, false);
        if(Keyboard::isKeyPressed(Keyboard::A)) GameSettings::world.movePlayer(-0.2f, 0.f, false);
        // shovel - right button
        // kopary
        if(Keyboard::isKeyPressed(Keyboard::S)) GameSettings::world.movePlayer(0.f, 0.2f, false);

        // UPDATE GAME LOGIC
        GameSettings::world.update();

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
