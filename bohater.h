#ifndef BOHATER_H
#define BOHATER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "obiekt.h"
#include "TextureManager.h"
using namespace std;
using namespace sf;

class bohater : public Postac{
public:
    bohater(){
        setTexture(TextureManager::getInstance().getTexture(text_path_idle));
        setTextureRect(IntRect(3, 13, 18, 34));
    };
    void change_frame(int &frame_count) {
        // funkcja zmiany klatki animacji
        if (is_moving) { // animacja biegu
            setTexture(TextureManager::getInstance().getTexture(text_path_run));

            int fr = frame_count % 6 + 1;
            switch (fr) {
            case 1: setTextureRect(IntRect(5, 16, 19, 31)); break;
            case 2: setTextureRect(IntRect(52, 17, 18, 30)); break;
            case 3: setTextureRect(IntRect(98, 17, 27 ,30)); break;
            case 4: setTextureRect(IntRect(144, 16, 31, 31)); break;
            case 5: setTextureRect(IntRect(195, 17, 22, 30)); break;
            case 6: {setTextureRect(IntRect(246, 17, 18, 30));frame_count = 0; break;}
            }
        }
        else { // animacja dla stania w miejscu
            setTexture(TextureManager::getInstance().getTexture(text_path_idle));
            int fr = frame_count % 4 + 1;
            switch (fr) {
            case 1: setTextureRect(IntRect(3, 13, 18 ,34)); break;
            case 2: setTextureRect(IntRect(51, 13, 19, 34)); break;
            case 3: setTextureRect(IntRect(98, 13, 19, 34)); break;
            case 4: {setTextureRect(IntRect(146, 13, 19, 34)); frame_count = 0; break;}
            }
        }
        reset_origin_point();
    }
    vector<obiekt> get_obiekty(){
        return inventory;
    }
    void set_is_running(const bool &temp){
        is_running = temp;
    }

private:
    int hp;

    bool is_running=false;

    vector<obiekt> inventory;
    const string text_path_run="assets\\bohater\\Cyborg_run.png";
    const string text_path_idle="assets\\bohater\\Cyborg_idle.png";

};
#endif // BOHATER_H
