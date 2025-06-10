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
    obiekt (const string &path){
        setTexture(TextureManager::getInstance().getTexture(path));
    }
    bool get_can_open_doors(){
        return can_open_doors;
    }
    bool get_can_hide_in(){
        return can_hide_in;
    }

protected:

    Texture texture;
    bool can_open_doors=false;
    bool can_hide_in=false;
    bool can_pick_up=false; //czy mozna podniesc przedmiot (szafy nie mozna np.)
};
#endif // OBIEKT_H
