#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "bohater.h"
#include "Struct_promien_slyszenia.h"
#include "dzwiek.h"
#include "potwor.h"
#include <memory>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;
using namespace sf;

template<typename T>
void set_proper_scale(unique_ptr<T> &s, const float &x_ratio, const float &y_ratio){
//gdy użyta zabezpiecza rysowane elementy
// przed rozciągnięciem/ściśnięciem przy odpaleniu programu na monitorze o innej rozdzielczości niż programisty
    const float x=s->getScale().x;
    const float y=s->getScale().y;
    s->setScale(x*x_ratio,y*y_ratio);
}

template<typename T>
void reset_origin_point(unique_ptr<T> &s){
    s->setOrigin(s->getLocalBounds().width / 2.f, s->getLocalBounds().height / 2.f);
}
template<typename T>
void reset_origin_point(T* &s){
    s->setOrigin(s->getLocalBounds().width / 2.f, s->getLocalBounds().height / 2.f);
}

template<typename T>
bool is_colliding_with_wall(const sf::Image &image_sciany,
                            std::unique_ptr<T> &obj,
                            const direction &dir, float scaleX, float scaleY)
 {
    float posX = obj->getPosition().x / scaleX;
    float posY = obj->getPosition().y / scaleY;

    float offsetX = 0.f;
    float offsetY = 0.f;

    float width  = obj->getGlobalBounds().width / scaleX;
    float height = obj->getGlobalBounds().height / scaleY;

    if (dir == direction::up)
        offsetY = -height / 2.f;
    else if (dir == direction::down)
        offsetY = height / 2.f;
    else if (dir == direction::left)
        offsetX = -width / 2.f;
    else if (dir == direction::right)
        offsetX = width / 2.f;

    int pixelX = static_cast<int>(posX + offsetX);
    int pixelY = static_cast<int>(posY + offsetY);

    if (pixelX < 0 || pixelY < 0 ||
        pixelX >= static_cast<int>(image_sciany.getSize().x) ||
        pixelY >= static_cast<int>(image_sciany.getSize().y)) {
        return false;
    }

    Color pixel = image_sciany.getPixel(pixelX, pixelY);
    return pixel.a > 0;
}



void move_hero(unique_ptr<bohater> &hero, Time &elapsed,
               const float &Scale_ratioX, const float &Scale_ratioY,
               const Image &image,
                vector <unique_ptr<promien_slysz>> &promienie_sluchu, Time &czas_do_nowego_promienia, Dzwiek &dzwiek,const vector <unique_ptr<potwor>>& monster){
    bool is_moving = false;
    bool a = Keyboard::isKeyPressed(Keyboard::A);
    bool d = Keyboard::isKeyPressed(Keyboard::D);
    bool w = Keyboard::isKeyPressed(Keyboard::W);
    bool s = Keyboard::isKeyPressed(Keyboard::S);
    bool shift = Keyboard::isKeyPressed(Keyboard::LShift);


    if (shift) {hero->set_v_ratio(0.6f); hero->set_is_running(false);}
    else {hero->set_v_ratio(1.f); hero->set_is_running(true);}

    if (a){
        if (!d){
            hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::left, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            }
            else
            {
                is_moving = true;
            }
            hero->turn_left();

        }
    }
    if (d){
        if (!a){
            hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::right, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            }
            else
            {
                is_moving = true;
            }
            hero->turn_right();
        }
    }
    if (w){
        if (!s){
            hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::up, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            }
            else
            {
                is_moving = true;
            }
        }
    }
    if (s){
        if (!w){
            hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::down, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            }
            else
            {
                is_moving = true;
            }
        }
    }
    if (!a && !d && !s && !w){
        hero->animate(elapsed,direction::none,Scale_ratioX,Scale_ratioY);
        hero->set_is_running(false);
        is_moving = false;
    }
    if (!shift && is_moving && czas_do_nowego_promienia <= Time::Zero){
        unique_ptr<promien_slysz> r_sluchu = make_unique<promien_slysz>();
        r_sluchu->setRadius(1.f);
        reset_origin_point(r_sluchu);
        r_sluchu->setPosition(hero->getPosition().x,hero->getPosition().y);
        r_sluchu->setFillColor(Color(0,0,0,0));
        r_sluchu->setOutlineThickness(10.f);
        r_sluchu->setOutlineColor(Color(250,0,0));
        promienie_sluchu.emplace_back(std::move(r_sluchu));
        czas_do_nowego_promienia = seconds(0.5f);
    }
    if (is_moving)
    {
        dzwiek.start_chodzenie();
    }
    else
    {
        dzwiek.stop_chodzenie();
    }
    if (!monster.empty())
    {
        Vector2f hero_pos = hero->getPosition();
        Vector2f potwor_pos = monster[0]->getPosition();
        float distance = sqrt(pow(potwor_pos.x - hero_pos.x, 2) +
                            pow(potwor_pos.y - hero_pos.y, 2));

        dzwiek.update_serce_heartbeat(distance, 800.0f);
    }
}
template<typename T>
float distance_between(const unique_ptr<T> &obiekt1, const unique_ptr<T> &obiekt2 ){
    Vector2f poz1=obiekt1->getPosition();
    Vector2f poz2=obiekt2->getPosition();
    float dis=sqrt(pow((poz2.x-poz1.x),2) + pow(poz2.y-poz1.y,2));

    return dis;
}
template<typename T>
float distance_between_m(const unique_ptr<T> &obiekt1, const unique_ptr<T> &obiekt2 ){
    Vector2f poz1=obiekt1->getPosition();
    Vector2f poz2=obiekt2->getPosition();
    float dis=sqrt(pow((poz2.x-poz1.x),2) + pow(poz2.y-poz1.y,2));

    return dis;
}
float distance_between_m(const unique_ptr<obiekt>& obj1, const unique_ptr<bohater>& obj2) {
    Vector2f poz1 = obj1->getPosition();
    Vector2f poz2 = obj2->getPosition();
    return sqrt(pow(poz2.x - poz1.x, 2) + pow(poz2.y - poz1.y, 2));
}

#endif // FUNKCJE_H
