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
    potwor(){};
    void change_frame(int &frame_count) {
        if (is_moving) {
            int fr = frame_count % 5;
            switch (fr) {
                case 0: setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_1.png")); break;
                case 1: setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_2.png")); break;
                case 2: setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_3.png")); break;
                case 3: setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_4.png")); break;
                case 4: {setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_run_5.png")); frame_count = 0; break;}
            }
        } else {
            int fr = frame_count % 2;
            switch (fr){
                case 0: setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_idle_1.png")); break;
                case 1: {setTexture(TextureManager::getInstance().getTexture("assets\\potwor\\scifi_alien_idle_1.png")); frame_count = 0; break;}
            }
        }
        reset_origin_point();
    }

private:
    const string text_path_idle = "assets\\potwor\\scifi_alien_idle_1.png";
};
#endif // POTWOR_H
