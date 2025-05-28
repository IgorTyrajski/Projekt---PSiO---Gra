#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

enum direction {none, left, right, up, down};

class Postac : public Sprite{
public:
    Postac (){
        if (!texture.loadFromFile(text_path)) {
            cout << "Could not load texture" << endl;
        }
        setTexture(texture);
    }
    void animate(Time &elapsed, const direction &dir){ //funkcja animowania - przyjmuje czas i kierunek, w ktorym porusza sie obiekt
        const float t=elapsed.asSeconds();
        if (dir==direction::none){
            move (0.0,0.0);
        }
        if (dir==direction::left){
            x_speed=abs(x_speed)*(-1);
            move(x_speed*t,0);
        }
        else if (dir==direction::right){
            x_speed=abs(x_speed);
            move(x_speed*t,0);
        }
        else if (dir==direction::up){
            y_speed=abs(y_speed)*(-1);
            move(0,y_speed*t);
        }
        else if (dir==direction::down){
            y_speed=abs(y_speed);
            move(0,y_speed);
        }
    }
    void change_frame(int &frame_count){
        ///funkcja zmiany klatki animacji (jezeli wszysttkie klatki sa na jednym png/jpg 'u to trzeba uzupelnic
        ///granice pojedynczych klatek)
        int fr=frame_count%4+1;
        if (is_moving){
            if (fr==1){
                setTextureRect(IntRect( , , , ));
            }
            else if (fr==2){
                setTextureRect(IntRect( , , , ));
            }
            else if (fr==3){
                setTextureRect(IntRect(, , , ));
            }
            else if (fr==4){
                setTextureRect(IntRect(, , , ));
            }
            else if (fr==5){
                setTextureRect(IntRect(, , , , ));
            }
            else{
                setTextureRect(IntRect(, , , , ));
                frame_count=0;
            }
        }
        else{
            setTextureRect(IntRect(, , , , ));
        }
    }
protected:
    float x_speed; //do ustawienia
    float y_speed;
    Texture texture;
    string text_path=""; //sciezka do pliku z textura

    bool is_moving=false;
};
