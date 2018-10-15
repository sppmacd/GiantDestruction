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
    ScreenSettings::window.create(sf::VideoMode(ScreenSettings::X_BY_Y*500, 500), "Giant Destruction", Style::Default ^ Style::Resize);
    ScreenSettings::window.setView(ScreenSettings::currentWorldView);
    ScreenSettings::window.setFramerateLimit(60);

    while (ScreenSettings::window.isOpen())
    {
        Event event;
        while (ScreenSettings::window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                ScreenSettings::window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)) GameSettings::world.movePlayer(1.f, 0.f);

        ScreenSettings::window.clear();

        ScreenSettings::windowSize = Vector2u(ScreenSettings::X_BY_Y*500, 500);
        GameSettings::world.update();
        ScreenSettings::window.setView(ScreenSettings::currentWorldView);
        ScreenRenderer::drawWorld(); //draw the box2d world

        ScreenSettings::window.display();
    }

    return 0;
}
