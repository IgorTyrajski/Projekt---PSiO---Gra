#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
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
    vector<Postac*> postacie;
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
    Vector2u windowSize = window.getSize();
    //////////////// background/////////////////////////////
    unique_ptr<Sprite> background = make_unique<Sprite>();
    Texture& bgTexture = TextureManager::getInstance().getTexture("assets\\mapa\\tlo.png");
    bgTexture.setRepeated(true);
    background->setTexture(bgTexture);
    background->setScale(1,1);
    background->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    set_proper_scale(background, Scale_ratioX,Scale_ratioY);
    to_draw.push_back(background.get());
    /////////////////////////////////////////////////
    /////floor/////////////////////////
    unique_ptr<Sprite> floor = make_unique<Sprite>();
    Texture& flTexture = TextureManager::getInstance().getTexture("assets\\mapa\\podloga.png");
    floor->setTexture(flTexture);
    floor->setPosition(windowSize.x/2,windowSize.y/2);
    floor->setScale(1,1);
    set_proper_scale(floor, Scale_ratioX,Scale_ratioY);
    reset_origin_point(floor);
    to_draw.push_back(floor.get());
    ////////////////////////////////////////////////
    ///////////sciany///////////////////////
    Image image_sciany;
    image_sciany.loadFromFile("assets\\mapa\\sciany.png"); //potrzebne do kolizji (in progress)

    auto sciany = make_unique<Sprite>();
    Texture& scTexture = TextureManager::getInstance().getTexture("assets\\mapa\\sciany.png");
    sciany->setTexture(scTexture);
    sciany->setPosition(windowSize.x/2,windowSize.y/2);
    sciany->setScale(1,1);
    set_proper_scale(sciany, Scale_ratioX,Scale_ratioY);
    reset_origin_point(sciany);
    to_draw.push_back(sciany.get());
    ///////////////////////////////////
    ////////////////// bohater /////////////////////narazie orientacyjnie
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(300,windowSize.y/2);
    hero->setScale(1.2f,1.2f);
    hero->set_v_ratio(1.f);
    hero->set_x_speed(120.f);
    hero->set_y_speed(120.f);
    hero->reset_origin_point();
    set_proper_scale(hero,Scale_ratioX,Scale_ratioY);
    postacie.push_back(hero.get());
    /////zasłona by bohater widział tylko to co ma widzieć/////
    //unique_ptr<Sprite> fog_of_war = make_unique<Sprite>();
    //Texture& fowTexture = TextureManager::getInstance().getTexture("assets\\bohater\\fog_of_war.png");
    //fog_of_war->setTexture(fowTexture);
    //reset_origin_point(fog_of_war);
    //fog_of_war->setPosition(hero->getPosition());
    //to_draw.push_back(fog_of_war.get());

    Shader fog_of_war;
    fog_of_war.loadFromFile("assets\\bohater\\mask.frag", Shader::Fragment);

    fog_of_war.setUniform("lightCenter", Glsl::Vec2(hero->getPosition()));
    fog_of_war.setUniform("lightRadius", 300.f);
    float aktualny_promien=300.f;
    RectangleShape mask(Vector2f(windowSize.x, windowSize.y));
    mask.setFillColor(Color::Black);
    window.draw(mask, &fog_of_war);
    ////////////////////////////////////////////////
    int frame_count=0, frame_count_h=0; //frame counter bohatera

    while (window.isOpen()) {
        Time elapsed=clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        ////////////ruszanie///////////

        move_hero(hero,elapsed,Scale_ratioX,Scale_ratioY,image_sciany);
        //fog_of_war->setPosition(hero->getPosition());
        fog_of_war.setUniform("lightCenter", hero->getPosition());
        fog_of_war.setUniform("lightRadius", aktualny_promien);
        if (frame_count%10+1>9){
            frame_count_h++;
        }
        hero->change_frame(frame_count_h);
        ///////////////////////////////
        ///////////DRAWING/////////////
        for (auto &d : to_draw){
            window.draw(*d);
        }
        for (auto &d : postacie){
            window.draw(*d);
        }
        window.draw(mask, &fog_of_war);
        window.display();
        frame_count++;
    }

    return 0;
}
