#ifndef POSTAC_H
#define POSTAC_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureManager.h"
using namespace std;
using namespace sf;

enum direction {none, left, right, up, down};
enum direction_adv {left_up, right_up, left_down, right_down};

class Postac : public Sprite{
public:
    Postac (){
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
    };
    void animate(const Time &elapsed, const direction &dir, const float &x_ratio,const float &y_ratio){ //funkcja animowania - przyjmuje czas i kierunek, w ktorym porusza sie obiekt
        const float t=elapsed.asSeconds();
        if (dir==direction::none){
            move (0.0,0.0);
            is_moving=false;
        }
        if (dir==direction::left){
            x_speed=abs(x_speed)*(-1);
            move(x_speed*t*v_ratio*x_ratio,0);
            is_moving=true;
        }
        else if (dir==direction::right){
            x_speed=abs(x_speed);
            move(x_speed*t*v_ratio*x_ratio,0);
            is_moving=true;
        }
        else if (dir==direction::up){
            y_speed=abs(y_speed)*(-1);
            move(0,y_speed*t*v_ratio*y_ratio);
            is_moving=true;
        }
        else if (dir==direction::down){
            y_speed=abs(y_speed);
            move(0,y_speed*t*v_ratio*y_ratio);
            is_moving=true;
        }
    }
    void turn_left(){
        Vector2f scale = getScale();
        setScale(-abs(scale.x), scale.y);
    }
    void turn_right(){
        Vector2f scale = getScale();
        setScale(abs(scale.x), scale.y);
    }
    void reset_origin_point(){
        setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
    }
    void set_x_speed(const float &xs){
        x_speed=xs;
    }
    void set_y_speed(const float &ys){
        y_speed=ys;
    }
    void set_v_ratio(const float &ratio){
        v_ratio=ratio;
    }

protected:
    float x_speed; //do ustawienia
    float y_speed;
    float v_ratio; //przelicznik predkosci
    bool is_moving=false;
};
#endif // POSTAC_H
