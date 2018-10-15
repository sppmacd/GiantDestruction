#include "config.hpp"

#include "Player.h"

using namespace std;

// variables and constants //

namespace ScreenSettings
{
const float X_BY_Y = 16.f/9.f;
Vector2u windowSize;
View currentWorldView;
}

namespace GameSettings
{
b2Body* playerB2Body;
Player* player;
}

/////////////////////////////

int main()
{
    playerPos.x
    currentWorldView.setCenter(playerPos);
    RenderWindow window(sf::VideoMode(X_BY_Y*500, 500), "Giant Destruction");


    shape.setFillColor(Color::Green);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
