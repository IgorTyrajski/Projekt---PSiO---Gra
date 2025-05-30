#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "postac.h"

using namespace std;
using namespace sf;

class bohater : public Postac{
public:
    bohater():Postac(text_path_idle){};
    void change_frame(int &frame_count){
        //funkcja zmiany klatki animacji
        if (is_moving){ //animacja biegu
            if (!texture.loadFromFile(text_path_run)) {
                cout << "Could not load texture" << endl;
            }
            setTexture(texture);
            int fr=frame_count%6+1;
            if (fr==1){
                setTextureRect(IntRect(5, 16, 19, 31));
            }
            else if (fr==2){
                setTextureRect(IntRect(52, 17, 18, 30));
            }
            else if (fr==3){
                setTextureRect(IntRect(98, 17, 27 ,30));
            }
            else if (fr==4){
                setTextureRect(IntRect(144, 16, 31, 31));
            }
            else if (fr==5){
                setTextureRect(IntRect(195, 17, 22, 30));
            }
            else{
                setTextureRect(IntRect(246, 17, 18, 30));
                frame_count=0;
            }
        }
        else{ //animacja dla stania w miejscu
            if (!texture.loadFromFile(text_path_idle)) {
                cout << "Could not load texture" << endl;
            }
            setTexture(texture);
            int fr=frame_count%4+1;
            if (fr==1){
                setTextureRect(IntRect(3, 13, 18 ,34));
            }
            else if (fr==2){
                setTextureRect(IntRect(51, 13, 19, 34));
            }
            else if (fr==3){
                setTextureRect(IntRect(98, 13, 19, 34));
            }
            else{
                setTextureRect(IntRect(146, 13, 19, 34));
                frame_count=0;
            }
        }
    }
private:
    int hp;

    const string text_path_run="assets\\bohater\\Cyborg_run.png";
    const string text_path_idle="assets\\bohater\\Cyborg_idle.png";

};
