#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>

#include "postac.h"
#include "bohater.h"
//#include "obiekt.h"
//#include "potwor.h"

using namespace std;
using namespace sf;

int main()
{
    vector<Drawable*> to_draw;
    Clock clock;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(desktop, "My window",Style::Fullscreen); //
    window.setFramerateLimit(60);
    //////////////// background/////////////////////////////

    auto background = make_unique<Sprite>();
    Texture& bgTexture = TextureManager::getInstance().getTexture("assets\\mapa\\floor.png");
    bgTexture.setRepeated(true);
    background->setTexture(bgTexture);
    background->setScale(3,3);
    Vector2u windowSize = window.getSize();
    background->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    to_draw.push_back(background.get());
    /////////////////////////////////////////////////
    ////////////////// bohater /////////////////////
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(300,400);

    ////////////////////////////////////////////////
    int frame_count=0;

    while (window.isOpen()) {
        Time elapsed=clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);

        ///////////DRAWING/////////////
        for (auto &d : to_draw){
            window.draw(*d);
        }
        window.draw(*hero);
        window.display();
        frame_count++;
    }
    return 0;
}
