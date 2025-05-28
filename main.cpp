#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"

using namespace std;
using namespace sf;

int main()
{
    Clock clock;
    RenderWindow window(VideoMode(1920, 900), "My window",Style::Fullscreen);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        Time elapsed=clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        window.display();
    }
    return 0;
}
