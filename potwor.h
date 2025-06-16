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
        setTexture(TextureManager::getInstance().getTexture(text_path_idle));
        is_moving=false;
        setTextureRect(IntRect(5, 16, 19, 31));
        reset_origin_point();
    };
    // W klasie potwor
    void set_proper_scale(float x_ratio, float y_ratio) {
        const float x = getScale().x;
        const float y = getScale().y;
        setScale(x * x_ratio, y * y_ratio);
    }

    void change_frame(int &frame_count, float scaleX, float scaleY) { //zmiana klatki animacji
        if (is_moving) {
            setTexture(TextureManager::getInstance().getTexture(text_path_run));;
            const int fr = frame_count % 5;
            switch (fr) {
            case 0: {setTextureRect(IntRect(16, 5, 105-16, 55-5));break;}
            case 1: {setTextureRect(IntRect(111, 10, 219-111, 55-10));break;}
            case 2: {setTextureRect(IntRect(230, 0, 320-230, 55-0));break;}
            case 3: {setTextureRect(IntRect(339, 0, 396-339, 56-0));break;}
            case 4: {setTextureRect(IntRect(404, 0, 467-404, 55-0)); frame_count = 0;break;}
            }


        } else {

            const int fr = frame_count % 5 + 1;
            switch (fr) {
            case 3: {setTextureRect(IntRect(4, 2, 68-4, 56-4)); break;}
            case 5: {setTextureRect(IntRect(73, 4, 133-73, 56-4));break;}
            }
        }
        setScale(1,1);
        this->set_proper_scale(scaleX, scaleY);
        reset_origin_point();
    }

private:

    const string text_path_run="assets\\potwor\\scifi_alien_run.png";
    const string text_path_idle="assets\\potwor\\scifi_alien_idle.png";
};
#endif // POTWOR_H
