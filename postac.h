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
            prevPosition=getPosition();
            move(x_speed*t*v_ratio*x_ratio,0);
            is_moving=true;
            looking_left=true;
            looking_right=false;
        }
        else if (dir==direction::right){
            x_speed=abs(x_speed);
            prevPosition=getPosition();
            move(x_speed*t*v_ratio*x_ratio,0);
            is_moving=true;
            looking_right=true;
            looking_left=false;
        }
        else if (dir==direction::up){
            y_speed=abs(y_speed)*(-1);
            prevPosition=getPosition();
            move(0,y_speed*t*v_ratio*y_ratio);
            is_moving=true;
            looking_top=true;
            looking_down=false;
        }
        else if (dir==direction::down){
            y_speed=abs(y_speed);
            prevPosition=getPosition();
            move(0,y_speed*t*v_ratio*y_ratio);
            is_moving=true;
            looking_down=true;
            looking_top=false;
        }
    }
    Vector2f getPrevPosition() const {
        return prevPosition;
    }

    void turn_left(){
        Vector2f scale = getScale();
        setScale(-abs(scale.x), scale.y);
        looking_left=true;
    }
    void turn_right(){
        Vector2f scale = getScale();
        setScale(abs(scale.x), scale.y);
        looking_right=true;
    }
    void turn_leftM(){
        Vector2f scale = getScale();
        setScale(abs(scale.x), scale.y);
        looking_left=true;
    }
    void turn_rightM(){
        Vector2f scale = getScale();
        setScale(-abs(scale.x), scale.y);
        looking_right=true;
    }
    float get_v_ratio(){
        return v_ratio;
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
    void set_is_running(const bool &temp){
        is_moving = temp;
    }
    bool get_is_running(){
        return is_moving;
    }
    bool get_is_looking_left(){
        return looking_left;
    }
    bool get_is_looking_right(){
        return looking_right;
    }
    bool get_is_looking_top(){
        return looking_top;
    }
    bool get_is_looking_down(){
        return looking_down;
    }
protected:
    float x_speed; //do ustawienia
    float y_speed;
    float v_ratio; //przelicznik predkosci
    bool is_moving=false;
    bool looking_right=false,looking_left=true,looking_top=false,looking_down=false;

    Vector2f prevPosition;
};
#endif // POSTAC_H
