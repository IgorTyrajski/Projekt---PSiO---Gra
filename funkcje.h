#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "bohater.h"
#include <memory>

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
    s->setOrigin(s->getLocalBounds().width / 2, s->getLocalBounds().height / 2);
}

bool is_colliding_with_wall(const Image &image_sciany, unique_ptr<bohater> &hero,const direction &dir){
    float localposx;
    float localposy;
    if (dir==direction::up){
        localposx=hero->getPosition().x;
        localposy=hero->getPosition().y-(hero->getGlobalBounds().width/1.8f);
    }
    if (dir==direction::down){
        localposx=hero->getPosition().x;
        localposy=hero->getPosition().y+(hero->getGlobalBounds().width);
    }
    if (dir==direction::left){
        localposx=hero->getPosition().x-(hero->getGlobalBounds().width/2.f);
        localposy=hero->getPosition().y;
    }
    if (dir==direction::right){
        localposx=hero->getPosition().x+(hero->getGlobalBounds().width/2.f);
        localposy=hero->getPosition().y;
    }



    Color pixel = image_sciany.getPixel(static_cast<unsigned>(localposx), static_cast<unsigned>(localposy));
    // Jeśli alfa > 0 – to znaczy, że jest ściana
    return pixel.a > 0;

}


void move_hero(unique_ptr<bohater> &hero,Time &elapsed, const float &Scale_ratioX,const float &Scale_ratioY, const Image image){
    bool a = Keyboard::isKeyPressed(Keyboard::A);
    bool d = Keyboard::isKeyPressed(Keyboard::D);
    bool w = Keyboard::isKeyPressed(Keyboard::W);
    bool s = Keyboard::isKeyPressed(Keyboard::S);
    if (a){
        if (!d){
            hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image,hero,direction::left)){
                hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            }
            hero->turn_left();
        }
    }
    if (d){
        if (!a){
            hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image,hero,direction::right)){
                hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            }
            hero->turn_right();
        }
    }
    if (w){
        if (!s){
            hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image,hero,direction::up)){
                hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            }
        }
    }
    if (s){
        if (!w){
            hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image,hero,direction::down)){
                hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            }
        }
    }
    if (!a && !d && !s && !w){
        hero->animate(elapsed,direction::none,Scale_ratioX,Scale_ratioY);
    }
}
#endif // FUNKCJE_H
