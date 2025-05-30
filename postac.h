#ifndef POSTAC_H
#define POSTAC_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureManager.h"
using namespace std;
using namespace sf;

enum direction {none, left, right, up, down};

class Postac : public Sprite{
public:
    Postac (){
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
    };
    void animate(const Time &elapsed, const direction &dir){ //funkcja animowania - przyjmuje czas i kierunek, w ktorym porusza sie obiekt
        const float t=elapsed.asSeconds();
        if (dir==direction::none){
            move (0.0,0.0);
        }
        if (dir==direction::left){
            x_speed=abs(x_speed)*(-1);
            move(x_speed*t*v_ratio,0);
        }
        else if (dir==direction::right){
            x_speed=abs(x_speed);
            move(x_speed*t*v_ratio,0);
        }
        else if (dir==direction::up){
            y_speed=abs(y_speed)*(-1);
            move(0,y_speed*t*v_ratio);
        }
        else if (dir==direction::down){
            y_speed=abs(y_speed);
            move(0,y_speed*t*v_ratio);
        }
    }
    void reset_origin_point(){
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
    }
    void set_x_speed(const int &xs){
        x_speed=xs;
    }
    void set_y_speed(const int &ys){
        y_speed=ys;
    }
    void set_v_ratio(const int &ratio){
        v_ratio=ratio;
    }

protected:
    float x_speed; //do ustawienia
    float y_speed;
    int v_ratio; //przelicznik predkosci


    bool is_moving=false;
};
#endif // POSTAC_H
