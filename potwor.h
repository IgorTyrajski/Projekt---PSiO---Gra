#ifndef POTWOR_H
#define POTWOR_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "TextureManager.h"

using namespace std;
using namespace sf;

class potwor : public Postac {
public:
    potwor(){
        idle1=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_idle_1.png");
        idle2=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_idle_2.png");
        run1=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_1.png");
        run2=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_2.png");
        run3=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_3.png");
        run4=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_4.png");
        run5=TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_5.png");
        is_moving=false;
        setTexture(idle1);
        reset_origin_point();
    };
    // W klasie potwor
    void set_proper_scale(float x_ratio, float y_ratio) {
        float x = getScale().x;
        float y = getScale().y;
        setScale(x * x_ratio, y * y_ratio);
    }

    void change_frame(int &frame_count, float scaleX, float scaleY) {
        if (is_moving) {
            int fr = frame_count % 5;
            switch (fr) {
            case 0: {setTexture(run1); break;}
            case 1: {setTexture(run2); break;}
            case 2: {setTexture(run3); break;}
            case 3: {setTexture(run4); break;}
            case 4: {setTexture(run5); frame_count = 0; break;}
            }
            if (is_left) turn_left();
            if (is_right) turn_right();
        } else {
            int fr = frame_count % 5 + 1;
            switch (fr) {
            case 3: {setTexture(idle1); break;}
            case 5: {setTexture(idle2); frame_count = 0; break;}
            }
        }
        setScale(1,1);
        this->set_proper_scale(scaleX, scaleY);
        reset_origin_point();
    }

private:

    Texture idle1, idle2;
    Texture run1, run2,run3,run4,run5;
};
#endif // POTWOR_H
