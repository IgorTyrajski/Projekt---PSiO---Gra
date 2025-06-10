#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef MOVE_MONSTER_H
#define MOVE_MONSTER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "bohater.h"
#include "funkcje.h"
#include "potwor.h"
#include "floor_square.h"
#include "Struct_promien_slyszenia.h"

#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <cmath>

using namespace std;
using namespace sf;

float distance_between(const unique_ptr<potwor> &obiekt1, const unique_ptr<bohater> &obiekt2 ){
    Vector2f poz1=obiekt1->getPosition();
    Vector2f poz2=obiekt2->getPosition();
    float dis=sqrt(pow((poz2.x-poz1.x),2) + pow(poz2.y-poz1.y,2));

    return dis;
}

bool check_if_hero_visible(const unique_ptr<potwor> &monster,const unique_ptr<bohater> &hero, const Image &image, const float &scale){
    const float seeing_range=600.f/scale;
    const float seeing_angle=70.0f;
    float base_angle;
    const float r_dis=distance_between(monster,hero);
    Vector2f delta = monster->getPosition() - hero->getPosition();
    float r_angle = atan2(delta.y, delta.x) * 180.f / M_PI; // wynik w stopniach

    bool a=monster->get_is_looking_left();
    bool d=monster->get_is_looking_right();
    bool w=monster->get_is_looking_top();
    bool s=monster->get_is_looking_down();
    if (a) base_angle=180.f;
    if (d) base_angle=0.f;
    if (w) base_angle=90.f;
    if (s) base_angle=-90.f;

    if (r_dis>seeing_range) return false;
    float angle_diff = r_angle - base_angle;
    while (angle_diff > 180.f) angle_diff -= 360.f;
    while (angle_diff < -180.f) angle_diff += 360.f;
    if (abs(angle_diff) > seeing_angle) return false;

    Vector2f start = monster->getPosition();
    Vector2f end = hero->getPosition();
    const int steps = 300;
    for (int i = 1; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        float x = start.x + (end.x - start.x) * t;
        float y = start.y + (end.y - start.y) * t;

        // Sprawdź, czy (x, y) mieści się w rozmiarze obrazka
        if (x < 0 || y < 0 || x >= image.getSize().x || y >= image.getSize().y) continue;

        Color pixel = image.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));
        if (pixel.a > 0) return false; // przeszkoda zasłania
    }
    return true;
}




template<typename T>
float distance_between_p(T* &obiekt1, T* &obiekt2 ){
    Vector2f poz1=obiekt1->getPosition();
    Vector2f poz2=obiekt2->getPosition();
    float dis=sqrt(pow((poz2.x-poz1.x),2) + pow(poz2.y-poz1.y,2));

    return dis;
}

bool is_wall(const sf::Image &image_sciany,
             floor_square* &obj,
             const float &scaleX, const float &scaleY)
{
    float posX = obj->getPosition().x / scaleX;
    float posY = obj->getPosition().y / scaleY;

    float offsetX = 0.f;
    float offsetY = 0.f;

    int pixelX = static_cast<int>(posX + offsetX);
    int pixelY = static_cast<int>(posY + offsetY);

    if (pixelX < 0 || pixelY < 0 ||
        pixelX >= static_cast<int>(image_sciany.getSize().x) ||
        pixelY >= static_cast<int>(image_sciany.getSize().y)) {
        return false;
    }

    Color pixel = image_sciany.getPixel(pixelX, pixelY);
    return pixel.a > 0;
}

vector<floor_square*> create_path(const vector<floor_square*> &tales, floor_square* &hero_pos, floor_square* &monster_pos){
    vector<floor_square*> path;
    floor_square *current= monster_pos;
    monster_pos->set_local(0.0f);
    monster_pos->set_global(distance_between_p(monster_pos,hero_pos));

    list<floor_square*> listNotTestedTales;
    listNotTestedTales.push_back(monster_pos);

    while (!listNotTestedTales.empty()){
        listNotTestedTales.sort([](floor_square *obj1, floor_square *obj2){
            return obj1->get_global()<obj2->get_global();
        });

        while (!listNotTestedTales.empty() && listNotTestedTales.front()->get_was_visited()){
            listNotTestedTales.pop_front();
        }
        if (listNotTestedTales.empty()) break;

        current=listNotTestedTales.front();
        current->set_was_visited(true);
        for (floor_square* &n : current->get_neighbours()){
            if (!n->get_was_visited() && !n->get_is_wall()){
                listNotTestedTales.push_back(n);
            }
            float possLowerGoal = current->get_local() + distance_between_p(current,n);
            if (possLowerGoal < n->get_local()){
                n->set_parent(current);
                n->set_local(possLowerGoal);
                n->set_global(n->get_local()+distance_between_p(n,hero_pos));
            }
        }
    }
    if (monster_pos!=nullptr){
        floor_square* f=hero_pos;
        while (f->get_parent()!=nullptr){
            f=f->get_parent();
            f->setFillColor(sf::Color(0, 0, 255, 128));
            path.push_back(f);
        }
    }
    return path;

}


bool check_if_hero_hearable(const vector <unique_ptr<promien_slysz>> &promienie,const unique_ptr<potwor> &monster){
    for (auto &p : promienie){
        if (p->getGlobalBounds().intersects(monster->getGlobalBounds())){
            return true;
        }
    }
    return false;
}
vector<floor_square*> create_floor(const Image &image,
                                    const float &scaleX, const float &scaleY) {
    const int baseTileSize = 21;
    vector<floor_square*> result;

    const int maxX = image.getSize().x;
    const int maxY = image.getSize().y;

    for (int x = 0; x + baseTileSize <= maxX; x += baseTileSize) {
        for (int y = 0; y + baseTileSize <= maxY; y += baseTileSize) {
            floor_square* tile = new floor_square();
            tile->setSize(Vector2f(baseTileSize * scaleX, baseTileSize * scaleY));
            tile->setPosition(x * scaleX, y * scaleY);
            tile->setFillColor(Color(200, 200, 200, 128));
            tile->setOutlineColor(Color::Black);
            tile->setOutlineThickness(1);
            reset_origin_point(tile);
            bool isWall = is_wall(image, tile, scaleX, scaleY);
            tile->set_is_wall(isWall);
            if (isWall) {
                tile->setFillColor(Color(255, 0, 0, 128));
            }
            result.push_back(tile);
        }
    }
    for (size_t i =0; i<result.size();i++){
        for (size_t j =0; j<result.size();j++){
            float dx = abs(result[i]->getPosition().x - result[j]->getPosition().x);
            float dy = abs(result[i]->getPosition().y - result[j]->getPosition().y);

            const float eps = 0.001f;
            if ((abs(dx - baseTileSize * scaleX) < eps && dy < eps) ||
                (abs(dy - baseTileSize * scaleY) < eps && dx < eps))

            result[i]->add_neighbour(result[j]);
        }

    }
    return result;
}

void move_monster(unique_ptr<potwor> &monster,vector<floor_square*> path, Time &elapsed,
                  const float &Scale_ratioX, const float &Scale_ratioY){
    const float e = 5.f;
    const float scaled_e_x = e * Scale_ratioX;
    const float scaled_e_y = e * Scale_ratioY;
    if (!path.empty()){
    bool a = path[path.size()-2]->getPosition().x + scaled_e_x < monster->getPosition().x;
    bool d = path[path.size()-2]->getPosition().x - scaled_e_x > monster->getPosition().x;
    bool w = path[path.size()-2]->getPosition().y + scaled_e_y < monster->getPosition().y;
    bool s = path[path.size()-2]->getPosition().y - scaled_e_y > monster->getPosition().y;

    if (a){
        monster->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            monster->turn_right(); //dla potwora jest odwrotnie "jak w lewo to w prawo"
    }
    else if (d){
        monster->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            monster->turn_left(); //dla potwora jest odwrotnie "jak w prawo to w lewo"
    }
    if (w){
        monster->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
    }
    else if (s){
        monster->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
    }
    monster->set_is_running(true);
}
}





#endif // MOVE_MONSTER_H
