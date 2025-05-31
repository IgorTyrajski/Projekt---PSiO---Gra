#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>

#include "funkcje.h"
#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"

using namespace std;
using namespace sf;

int main()
{
    vector<Sprite*> to_draw;
    Clock clock;
    ////////window///////////////
    VideoMode desktop = VideoMode::getDesktopMode();
    const float baseX = 1920.f;
    const float baseY = 1080.f;
    const float window_X=desktop.width;
    const float window_Y=desktop.height;
    const float Scale_ratioX=window_X/baseX;
    const float Scale_ratioY=window_Y/baseY;
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
    set_proper_scale(background, Scale_ratioX,Scale_ratioY);
    to_draw.push_back(background.get());
    /////////////////////////////////////////////////
    ////////////////// bohater /////////////////////
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(300,400);
    to_draw.push_back(hero.get());
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
        window.display();
        frame_count++;
    }
    return 0;
}
