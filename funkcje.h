#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;
using namespace sf;

template<typename T>
void set_proper_scale(unique_ptr<T> &s, const float x_ratio, const float y_ratio){
//gdy użyta zabezpiecza rysowane elementy
// przed rozciągnięciem/ściśnięciem przy odpaleniu programu na monitorze o innej rozdzielczości niż programisty
    const float x=s->getScale().x;
    const float y=s->getScale().y;
    s->setScale(x*x_ratio,y*y_ratio);
}

#endif // FUNKCJE_H
