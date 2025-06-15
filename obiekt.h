#ifndef OBIEKT_H
#define OBIEKT_H


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureManager.h"
using namespace std;
using namespace sf;

class obiekt : public Sprite{
public:

    bool get_can_hide_in(){
        return can_hide_in;
    }
    void set_can_hide_in(const bool &temp){
        can_hide_in=temp;
    }
    bool get_can_pick_up(){
        return can_pick_up;
    }
    void set_can_pick_up(const bool &temp){
        can_pick_up=temp;
    }
protected:

    Texture texture;
    bool can_hide_in=false;
    bool can_pick_up=false; //czy mozna podniesc przedmiot (szafy nie mozna np.)
};
#endif // OBIEKT_H
