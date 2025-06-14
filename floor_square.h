#ifndef FLOOR_SQUARE_H
#define FLOOR_SQUARE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <memory>
#include <vector>

#include <cmath>

using namespace std;
using namespace sf;


class floor_square : public RectangleShape {
public:
    bool get_is_wall(){
        return is_wall;
    }
    void set_is_wall(const bool &temp) {
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
    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }
    void set_x(const int &temp){
        x=temp;
    }
    void set_y(const int &temp){
        y=temp;
    }

private:
    int x,y;
    vector<floor_square*> neighbours;
    floor_square* parent=nullptr;
    bool was_visited=false;
    bool is_wall=false;
    float local_score=INFINITY;
    float global_score=INFINITY;
};

#endif // FLOOR_SQUARE_H
