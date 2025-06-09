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
#include <list>
#include <cmath>

using namespace std;
using namespace sf;

class floor_square : public RectangleShape {
public:
    bool get_is_wall(){
        return is_wall;
    }
    void set_is_wall(const bool & temp) {
        is_wall=temp;
    }
    void set_was_visited(const bool &temp){
        was_visited=temp;
    }
    bool get_was_visited(){
        return was_visited;
    }
    vector<floor_square*> get_neighbours (){
        return neighbours;
    }
    void add_neighbour(floor_square* &temp){
        neighbours.push_back(temp);
    }
    floor_square* get_parent(){
        return parent;
    }
    void set_parent(floor_square* &temp){
        parent=temp;
    }
    void set_local(const float &temp){
        local_score=temp;
    }
    void set_global(const float &temp){
        global_score=temp;
    }
    float get_local(){
        return local_score;
    }
    float get_global(){
        return global_score;
    }
    void reset_astar_state() {
        was_visited = false;
        parent = nullptr;
        local_score = INFINITY;
        global_score = INFINITY;
    }


private:
    vector<floor_square*> neighbours;
    floor_square* parent=nullptr;
    bool was_visited=false;
    bool is_wall=false;
    float local_score=INFINITY;
    float global_score=INFINITY;
};

template<typename T>
float distance_between(T* &obiekt1, T* &obiekt2 ){
    Vector2f poz1=obiekt1->getPosition();
    Vector2f poz2=obiekt2->getPosition();
    float dis=sqrt(pow((poz2.x-poz1.x),2) + pow(poz2.y-poz1.y,2));

    return dis;
}

bool is_wall(const sf::Image &image_sciany,
             floor_square* &obj,
             float scaleX, float scaleY)
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
    monster_pos->set_global(distance_between(monster_pos,hero_pos));

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
            float possLowerGoal = current->get_local() + distance_between(current,n);
            if (possLowerGoal < n->get_local()){
                n->set_parent(current);
                n->set_local(possLowerGoal);
                n->set_global(n->get_local()+distance_between(n,hero_pos));
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
                  const Image &image,const float &Scale_ratioX, const float &Scale_ratioY,
                  float &run_ratio){
    const float e = 5.f;
    const float scaled_e_x = e * Scale_ratioX;
    const float scaled_e_y = e * Scale_ratioY;

    bool a = path[path.size()-2]->getPosition().x + scaled_e_x < monster->getPosition().x;
    bool d = path[path.size()-2]->getPosition().x - scaled_e_x > monster->getPosition().x;
    bool w = path[path.size()-2]->getPosition().y + scaled_e_y < monster->getPosition().y;
    bool s = path[path.size()-2]->getPosition().y - scaled_e_y > monster->getPosition().y;

    if (a){
        monster->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
        if (is_colliding_with_wall(image, monster, direction::left, Scale_ratioX, Scale_ratioY))
        {
            //monster->set_v_ratio(2.f);
            //monster->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            //monster->set_v_ratio(0.5f);
        }
        monster->turn_left();
    }
    else if (d){
        monster->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
        if (is_colliding_with_wall(image, monster, direction::right, Scale_ratioX, Scale_ratioY))
        {
            //monster->set_v_ratio(2.f);
            //monster->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            //monster->set_v_ratio(1.f);
        }
        monster->turn_right();
    }
    if (w){
        monster->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
        if (is_colliding_with_wall(image, monster, direction::up, Scale_ratioX, Scale_ratioY))
        {
            //monster->set_v_ratio(2.f);
            //monster->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            //monster->set_v_ratio(1.f);
        }
    }
    else if (s){
        monster->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
        if (is_colliding_with_wall(image, monster, direction::down, Scale_ratioX, Scale_ratioY))
        {
            //monster->set_v_ratio(2.f);
            //monster->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            //monster->set_v_ratio(0.5f);
    }
    monster->set_is_running(true);
}
}





#endif // MOVE_MONSTER_H
