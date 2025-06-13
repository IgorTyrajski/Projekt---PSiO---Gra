#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>


#include "move_monster.h"
#include "funkcje.h"
#include "postac.h"
#include "bohater.h"
//#include "obiekt.h"
#include "potwor.h"
#include "dzwiek.h"
#include "Struct_promien_slyszenia.h"
#include "TextureManager.h"



using namespace std;
using namespace sf;

int main()
{

    bool develop_mode=true; //tryb "deweloperski" wylacza np. mgle wojny tak aby bylo widac co sie dzieje
    vector<Sprite*> to_draw;
    vector<Postac*> postacie;
    vector <unique_ptr<promien_slysz>> promienie_sluchu; //tu sa przechowywane dzwieki tupniecia bohatera

    Dzwiek dzwiek;
    dzwiek.load_serce_sound("assets/bohater/heartbeat.wav");
    dzwiek.start_serce_system();


    Clock clock;
    ////////window///////////////
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(desktop, "My window", Style::Fullscreen);

    Vector2u windowSize = window.getSize();

    const float baseX = 1920.f;
    const float baseY = 1080.f;
    const float window_X = static_cast<float>(windowSize.x);
    const float window_Y = static_cast<float>(windowSize.y);

    const float Scale_ratioX = window_X / baseX;
    const float Scale_ratioY = window_Y / baseY;
    const float Scale_general=(Scale_ratioX+Scale_ratioY)/2.f;
    vector<unique_ptr<potwor>> potwory;
    window.setFramerateLimit(60);
    if (develop_mode) {cout << "Okno: " << window_X << " x " << window_Y << endl; cout << "Skala X: " << Scale_ratioX << " Skala Y: " << Scale_ratioY << endl;}
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


    vector<floor_square*> floor_tile=create_floor(image_sciany,Scale_ratioX,Scale_ratioY);
    floor_square* hero_pos=nullptr;
    floor_square* goal=nullptr;
    floor_square* monster_pos=nullptr;
    vector<floor_square*> path;

    ///////////////////////////////////
    ///////////Potwor//////////////////
    unique_ptr<potwor>monster = make_unique<potwor>();
    monster->setPosition(windowSize.x/2.f,windowSize.y/2.f);
    monster->set_v_ratio(1.f);
    monster->set_x_speed(100.f);
    monster->set_y_speed(100.f);
    monster->setScale(0.7f,0.7f);
    monster->reset_origin_point();
    set_proper_scale(monster,Scale_ratioX,Scale_ratioY);
    postacie.push_back(monster.get());
    potwory.push_back(std::move(monster));


    unique_ptr<ConvexShape> cone = make_unique<ConvexShape>(); //stożek widoczności potwora
    cone->setFillColor(sf::Color(255, 255, 0, 80));

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
    dzwiek.load_chodzenie_sound("assets/bohater/ruch.wav");
    ///////////////////////////////////////
    /////////////////////////////////////////////////////
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

    hero_pos = nullptr;
    monster_pos = nullptr;
    goal=nullptr;


    ////////////////////////////////////////////////
    int frame_count1=0, frame_count2=0, frame_count_h=0, frame_count_m=0; //frame counter bohatera i potwora
    Time czas_do_nowego_promienia = Time::Zero;

    while (window.isOpen()) {
        Time elapsed=clock.restart();


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        ////////////ruszanie///////////

        move_hero(hero, elapsed, Scale_ratioX, Scale_ratioY, image_sciany, promienie_sluchu,czas_do_nowego_promienia,dzwiek,potwory);
        //fog_of_war->setPosition(hero->getPosition());
        fog_of_war.setUniform("lightCenter", hero->getPosition());
        fog_of_war.setUniform("lightRadius", aktualny_promien);

        int kl_h=10;
        if ((frame_count1%kl_h)+1==kl_h){
            frame_count_h++;
            hero->change_frame(frame_count_h);
        }


        for (auto &t : floor_tile){
            if (t->get_is_wall()) {
                t->setFillColor(Color(255, 0, 0, 200));  // czerwony
            } else {
                t->setFillColor(Color(200, 200, 200, 128));  // szary
            }
        }
        for (auto &t : floor_tile){
            if (t->getGlobalBounds().contains(hero->getPosition())) {
                hero_pos = t;
                break;
            }
        }
        for (auto &t : floor_tile){
            if (t->getGlobalBounds().contains(potwory[0]->getPosition())) {
                monster_pos = t;
                break;
            }
        }

        bool can_see = check_if_hero_visible(potwory[0],hero,image_sciany,Scale_general,cone);
        bool can_hear = check_if_hero_hearable(promienie_sluchu,potwory[0]);
        potwory[0]->set_v_ratio(1.f);

        if (can_see) {
            potwory[0]->set_v_ratio(potwory[0]->get_v_ratio() * 2.f);
            for (auto &tile : floor_tile) {
                tile->reset_astar_state();
            }
            goal = hero_pos;  // Cel = pozycja bohatera
        }
        // Jeśli nie widzi, ale słyszy
        else if (!promienie_sluchu.empty() && can_hear) {
            potwory[0]->set_v_ratio(potwory[0]->get_v_ratio() * 2.f);
            for (auto &t : floor_tile) {
                if (t->getGlobalBounds().contains(promienie_sluchu[0]->getPosition())) {
                    goal = t;  // Cel = pozycja dźwięku
                }
            }
            for (auto &tile : floor_tile) {
                tile->reset_astar_state();
            }
        }
        // Jeśli nie widzi i nie słyszy, ale ma już cel (np. ostatnią znaną pozycję bohatera)
        else if (goal != nullptr) {
            potwory[0]->set_v_ratio(1.f);  // Normalna prędkość
        }
        else {
            potwory[0]->set_v_ratio(1.f);
            goal = nullptr;  // Brak celu
        }

        // Generuj ścieżkę tylko jeśli jest cel i nie jest to pozycja potwora
        if (goal && goal != monster_pos) {
            path = create_path(floor_tile, goal, monster_pos);
        } else {
            path.clear();  // Brak ścieżki
        }
        if (path.size()==1){
            path.clear();
        }

        int kl_m=8;
        if ((frame_count2%kl_m)+1==kl_m){
            frame_count_m++;
            potwory[0]->change_frame(frame_count_m, Scale_ratioX, Scale_ratioY);
        }
        if (!path.empty()){
            move_monster(potwory[0],path,elapsed,Scale_ratioX,Scale_ratioY);
        }


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
        if (develop_mode) {
            window.draw(*cone);
            for (auto& p : promienie_sluchu) {
                if (p->get_pozostaly_czas() >= milliseconds(0)) {
                    p->set_pozostaly_czas(p->get_pozostaly_czas() - elapsed);
                    p->setRadius(p->getRadius() + 5.f);
                    reset_origin_point(p);
                    window.draw(*p);
                }
            }

            promienie_sluchu.erase(remove_if(
                promienie_sluchu.begin(), promienie_sluchu.end(), [](const unique_ptr<promien_slysz>& p) {
                    return p->get_pozostaly_czas() < seconds(0);}), promienie_sluchu.end());

            //////////rysowanie podglogi//////////

            for (auto &t : floor_tile){
                window.draw(*t);
            }

            for (auto &t : path) {
                window.draw(*t);
            }
            if (hero_pos) {
                hero_pos->setFillColor(Color(255, 255, 0, 128));
                window.draw(*hero_pos);
            }
            if (monster_pos) {
                //monster_pos->setFillColor(Color(0, 255, 0, 128));
                window.draw(*monster_pos);
            }
        }

        window.display();
        frame_count1++;
        frame_count2++;
        czas_do_nowego_promienia -= elapsed;

        if (potwory.empty()) window.close();

    }

    for (floor_square* ptr : floor_tile) {
        delete ptr;
    }
    return 0;
}
