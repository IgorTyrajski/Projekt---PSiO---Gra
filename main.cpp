#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>

#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"


using namespace std;
using namespace sf;

int main()
{
    vector<unique_ptr<Sprite>> to_draw;

    Clock clock;
    RenderWindow window(VideoMode(1920, 1080), "My window"); //Style::Fullscreen
    window.setFramerateLimit(60);
    //////////////// background/////////////////////////////
    Texture background_texture;
    if (!background_texture.loadFromFile("assets\\mapa\\TileSet.png")) {
        cerr << "Could not load texture" << endl;
        return 1;
    }
    background_texture.setRepeated(true);
    auto background = make_unique<Sprite>();
    background->setTexture(background_texture);
    background->setTextureRect(IntRect(0,0,96,96));
    to_draw.push_back(move(background));
    /////////////////////////////////////////////////
    ////////////////// bohater /////////////////////
    auto hero = make_unique<bohater>();
    hero->setPosition(300,400);
    to_draw.push_back(move(hero));
    ////////////////////////////////////////////////
    int frame_count=0;
    Time elapsed=clock.restart();
    while (window.isOpen()) {

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
