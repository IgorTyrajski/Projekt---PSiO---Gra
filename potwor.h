#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "postac.h"

using namespace std;
using namespace sf;

class potwor : public Postac{
public:
    potwor():Postac(text_path_idle){
        for (int i=1; i<=2;i++){
            Texture texture;
            if (!texture.loadFromFile("assets\\potwor\\scifi_alien_idle_"+to_string(i)+".png")) {
                cout << "Could not load texture" << endl;
            }
            idle.push_back(texture);
        }
        for (int i=1; i<=5;i++){
            Texture texture;
            if (!texture.loadFromFile("assets\\potwor\\scifi_alien_run_"+to_string(i)+".png")) {
                cout << "Could not load texture" << endl;
            }
            run.push_back(texture);
        }
    };
    void change_frame(int &frame_count){
        //funkcja zmiany klatki animacji

        if (is_moving){ //animacja biegu
            int fr=frame_count%5;
            setTexture(run[fr]);
            if (fr==4) frame_count=0;
        }
        else{ //animacja dla stania w miejscu
            int fr=frame_count%2;
            setTexture(idle[fr]);
            if (fr==1) frame_count=0;
        }
    }
private:
    vector<Texture> idle;
    vector<Texture> run;
    const string text_path_idle="assets\\potwor\\scifi_alien_idle_1.png";


};
