#ifndef POTWOR_H
#define POTWOR_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"

using namespace std;
using namespace sf;

class potwor : public Postac {
public:
    potwor() : Postac(text_path_idle) {
        // animacjie stania w miejscu
        if (!idle1.loadFromFile("assets\\potwor\\scifi_alien_idle_1.png")) {
            cout << "Could not load texture" << endl;
        }
        if (!idle2.loadFromFile("assets\\potwor\\scifi_alien_idle_2.png")) {
            cout << "Could not load texture" << endl;
        }
        // animacje biegu
        if (!run1.loadFromFile("assets\\potwor\\scifi_alien_run_1.png")) {
            cout << "Could not load texture" << endl;
        }
        if (!run2.loadFromFile("assets\\potwor\\scifi_alien_run_2.png")) {
            cout << "Could not load texture" << endl;
        }
        if (!run3.loadFromFile("assets\\potwor\\scifi_alien_run_3.png")) {
            cout << "Could not load texture" << endl;
        }
        if (!run4.loadFromFile("assets\\potwor\\scifi_alien_run_4.png")) {
            cout << "Could not load texture" << endl;
        }
        if (!run5.loadFromFile("assets\\potwor\\scifi_alien_run_5.png")) {
            cout << "Could not load texture" << endl;
        }
    }

    void change_frame(int &frame_count) {
        if (is_moving) {
            int fr = frame_count % 5;
            switch (fr) {
                case 0: setTexture(run1); break;
                case 1: setTexture(run2); break;
                case 2: setTexture(run3); break;
                case 3: setTexture(run4); break;
                case 4: {setTexture(run5); frame_count = 0; break;}
            }
        } else {
            int fr = frame_count % 2;
            switch (fr){
                case 0: setTexture(idle1); break;
                case 1: {setTexture(idle2); frame_count = 0; break;}
            }
        }
        reset_origin_point();
    }

private:
    Texture idle1, idle2;
    Texture run1, run2, run3, run4, run5;
    const string text_path_idle = "assets\\potwor\\scifi_alien_idle_1.png";
};
#endif // POTWOR_H
