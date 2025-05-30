#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"


using namespace std;
using namespace sf;

int main()
{
    Clock clock;
    RenderWindow window(VideoMode(1920, 1080), "My window",Style::Fullscreen);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        int frame_count=0;
        Time elapsed=clock.restart();

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        window.display();
        frame_count++;
    }
    return 0;
}
