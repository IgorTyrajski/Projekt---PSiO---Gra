#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

#include "funkcje.h"
#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"

using namespace std;
using namespace sf;

int main()
{
    bool develop_mode=true; //tryb "deweloperski" wylacza np. mgle wojny tak aby bylo widac co sie dzieje

    vector<Sprite*> to_draw;
    vector<Postac*> postacie;
    Clock clock;
    ////////window///////////////
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(desktop, "My window", Style::Fullscreen);

    // Po utworzeniu okna pobierz jego rozmiar:
    Vector2u windowSize = window.getSize();

    const float baseX = 1920.f;
    const float baseY = 1080.f;
    const float window_X = static_cast<float>(windowSize.x);
    const float window_Y = static_cast<float>(windowSize.y);

    const float Scale_ratioX = window_X / baseX;
    const float Scale_ratioY = window_Y / baseY;
    window.setFramerateLimit(60);
    if (!develop_mode) {cout << "Okno: " << window_X << " x " << window_Y << endl;}
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
    image_sciany.loadFromFile("assets\\mapa\\sciany.png"); //potrzebne do kolizji

    auto sciany = make_unique<Sprite>();
    Texture& scTexture = TextureManager::getInstance().getTexture("assets\\mapa\\sciany.png");
    sciany->setTexture(scTexture);
    sciany->setPosition(windowSize.x/2,windowSize.y/2);
    sciany->setScale(1,1);
    set_proper_scale(sciany, Scale_ratioX,Scale_ratioY);
    reset_origin_point(sciany);
    to_draw.push_back(sciany.get());
    ///////////////////////////////////
    ///////////Potwor//////////////////
    unique_ptr<potwor>monster = make_unique<potwor>();
    monster->setPosition(windowSize.x/2.f,windowSize.y/2.f);
    monster->setScale(1.f,1.f);
    monster->reset_origin_point();
    set_proper_scale(monster,Scale_ratioX,Scale_ratioY);
    postacie.push_back(monster.get());
    /////////////////////////////////////////////////
    ////////////////// bohater /////////////////////narazie orientacyjnie
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(windowSize.x/6.f,windowSize.y/2.f);
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
    //to wyzej to kod mgly wojny ale przy uzyciu sprite, to co jest nizej jest abrdziej fancy
    Shader fog_of_war;
    if (!fog_of_war.loadFromFile("assets\\bohater\\mask.frag", Shader::Fragment)) {
        cout << "Nie udalo sie zaladowac shadera!" << endl;
    }
    fog_of_war.setUniform("lightCenter", Glsl::Vec2(hero->getPosition()));
    fog_of_war.setUniform("lightRadius", 300.f);
    fog_of_war.setUniform("resolution", Glsl::Vec2(window.getSize()));

    float aktualny_promien=300.f;
    RectangleShape mask(Vector2f(windowSize.x, windowSize.y));
    mask.setFillColor(Color::Black);
    ////////////////////////////////////////////////
    int frame_count1=0, frame_count2=0, frame_count_h=0, frame_count_m=0; //frame counter bohatera

    while (window.isOpen()) {
        Time elapsed=clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        ////////////ruszanie///////////

        move_hero(hero, elapsed, Scale_ratioX, Scale_ratioY, image_sciany);
        //fog_of_war->setPosition(hero->getPosition());
        fog_of_war.setUniform("lightCenter", hero->getPosition());
        fog_of_war.setUniform("lightRadius", aktualny_promien);

        if (frame_count1%10+1==10){
            frame_count_h++;
        }
        hero->change_frame(frame_count_h);

        if (frame_count2%8+1==8){
            frame_count_m++;
        }
        monster->change_frame(frame_count_m, Scale_ratioX, Scale_ratioY);
        ///////////////////////////////
        ///////////DRAWING/////////////
        for (auto &d : to_draw){
            window.draw(*d);
        }
        for (auto &d : postacie){
            window.draw(*d);
        }
        if (!develop_mode){
            window.draw(mask, &fog_of_war);
        }



        window.display();
        frame_count1++;
        frame_count2++;
    }

    return 0;
}
