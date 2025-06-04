#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "postac.h"
#include "bohater.h"
#include <memory>

using namespace std;
using namespace sf;

template<typename T>
void set_proper_scale(unique_ptr<T> &s, const float &x_ratio, const float &y_ratio){
//gdy użyta zabezpiecza rysowane elementy
// przed rozciągnięciem/ściśnięciem przy odpaleniu programu na monitorze o innej rozdzielczości niż programisty
    const float x=s->getScale().x;
    const float y=s->getScale().y;
    s->setScale(x*x_ratio,y*y_ratio);
}

template<typename T>
void reset_origin_point(unique_ptr<T> &s){
    s->setOrigin(s->getLocalBounds().width / 2, s->getLocalBounds().height / 2);
}

bool is_colliding_with_wall(const sf::Image &image_sciany,
                            std::unique_ptr<bohater> &hero,
                            const direction &dir, float scaleX, float scaleY)
 {
    // Współrzędne bohatera przeskalowane do bazowej rozdzielczości (np. 1920x1080)
    float posX = hero->getPosition().x / scaleX;
    float posY = hero->getPosition().y / scaleY;

    float offsetX = 0.f;
    float offsetY = 0.f;

    // Rozmiar sprite'a bohatera również cofnięty do bazowej skali
    float width  = hero->getGlobalBounds().width / scaleX;
    float height = hero->getGlobalBounds().height / scaleY;

    // Wyznacz przesunięcie kierunkowe względem środka postaci
    if (dir == direction::up)
        offsetY = -height / 2.f;
    else if (dir == direction::down)
        offsetY = height / 2.f;
    else if (dir == direction::left)
        offsetX = -width / 2.f;
    else if (dir == direction::right)
        offsetX = width / 2.f;

    // Finalna pozycja do sprawdzenia kolizji w bazowej rozdzielczości
    int pixelX = static_cast<int>(posX + offsetX);
    int pixelY = static_cast<int>(posY + offsetY);

    // Zabezpieczenie przed wyjściem poza obrazek kolizji
    if (pixelX < 0 || pixelY < 0 ||
        pixelX >= static_cast<int>(image_sciany.getSize().x) ||
        pixelY >= static_cast<int>(image_sciany.getSize().y)) {
        return false;
    }

    // Pobranie koloru piksela z obrazu kolizji
    Color pixel = image_sciany.getPixel(pixelX, pixelY);

    // Kolizja = piksel z alfą > 0 (np. ściana, przeszkoda)
    return pixel.a > 0;
}



void move_hero(unique_ptr<bohater> &hero, Time &elapsed,
               const float &Scale_ratioX, const float &Scale_ratioY,
               const Image &image, float &run_ratio){
    bool a = Keyboard::isKeyPressed(Keyboard::A);
    bool d = Keyboard::isKeyPressed(Keyboard::D);
    bool w = Keyboard::isKeyPressed(Keyboard::W);
    bool s = Keyboard::isKeyPressed(Keyboard::S);
    bool shift = Keyboard::isKeyPressed(Keyboard::LShift);


    if (shift) {hero->set_v_ratio(0.6f); hero->set_is_running(false); run_ratio=1.3f;}
    else {hero->set_v_ratio(1.f); hero->set_is_running(true); run_ratio=1.f;}

    if (a){
        if (!d){
            hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::left, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            }
            hero->turn_left();
        }
    }
    if (d){
        if (!a){
            hero->animate(elapsed,direction::right,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::right, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::left,Scale_ratioX,Scale_ratioY);
            }
            hero->turn_right();
        }
    }
    if (w){
        if (!s){
            hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::up, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            }
        }
    }
    if (s){
        if (!w){
            hero->animate(elapsed,direction::down,Scale_ratioX,Scale_ratioY);
            if (is_colliding_with_wall(image, hero, direction::down, Scale_ratioX, Scale_ratioY))
            {
                hero->animate(elapsed,direction::up,Scale_ratioX,Scale_ratioY);
            }
        }
    }
    if (!a && !d && !s && !w){
        hero->animate(elapsed,direction::none,Scale_ratioX,Scale_ratioY);
    }
}
#endif // FUNKCJE_H
