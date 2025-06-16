#ifndef STRUCT_PROMIEN_SLYSZENIA_H
#define STRUCT_PROMIEN_SLYSZENIA_H

//służy do określenia pola słyszenia bohatera przez potwora
//rysowany jedynie w trybie developerskim


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class promien_slysz : public CircleShape{
public:
    promien_slysz(){
        pozostaly_czas=seconds(1);
    }

    Time get_pozostaly_czas(){
        return pozostaly_czas;
    }
    void set_pozostaly_czas(const Time &newT){
        pozostaly_czas=newT;
    }


private:
    Time pozostaly_czas;
};

#endif // STRUCT_PROMIEN_SLYSZENIA_H
