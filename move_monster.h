#ifndef MOVE_MONSTER_H
#define MOVE_MONSTER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "bohater.h"
#include "funkcje.h"
#include "potwor.h"

#include <memory>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;

void move_monster(unique_ptr<potwor> &monster, unique_ptr<bohater> &hero, Time &elapsed,
               const float &Scale_ratioX, const float &Scale_ratioY,
               const Image &image, float &run_ratio){

    Vector2f destination=hero->getPosition();
    Vector2f m_position = monster->getPosition();

    bool a = destination.x<m_position.x;
    bool d = destination.x>m_position.x;
    bool w = destination.y<m_position.y;
    bool s = destination.y>m_position.y;

    if (a){
        monster->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
        monster->turn_left();
    }
    else if (d){
        monster->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
        monster->turn_right();
    }
    if (w){
        monster->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
    }
    else if (s){
        monster->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
    }

}




#endif // MOVE_MONSTER_H
