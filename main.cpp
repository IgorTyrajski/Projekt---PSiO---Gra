<<<<<<< HEAD
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "funkcje.h"
#include "postac.h"
#include "bohater.h"
#include "obiekt.h"
#include "potwor.h"

using namespace std;
using namespace sf;

int main()
{
    vector<Sprite*> to_draw;
    vector<Postac*> postacie;
    Clock clock;
    ////////window///////////////
    VideoMode desktop = VideoMode::getDesktopMode();
    const float baseX = 1920.f;
    const float baseY = 1080.f;
    const float window_X=desktop.width;
    const float window_Y=desktop.height;
    const float Scale_ratioX=window_X/baseX;
    const float Scale_ratioY=window_Y/baseY;
    RenderWindow window(desktop, "My window",Style::Fullscreen); //
    window.setFramerateLimit(60);
    Vector2u windowSize = window.getSize();
    //////////////// background/////////////////////////////
    unique_ptr<Sprite> background = make_unique<Sprite>();
    Texture& bgTexture = TextureManager::getInstance().getTexture("assets\\mapa\\tlo.png");
    bgTexture.setRepeated(true);
    background->setTexture(bgTexture);
    background->setScale(1,1);
    background->setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    set_proper_scale(background, Scale_ratioX,Scale_ratioY);
    to_draw.push_back(background.get());
    /////////////////////////////////////////////////
    /////floor/////////////////////////
    unique_ptr<Sprite> floor = make_unique<Sprite>();
    Texture& flTexture = TextureManager::getInstance().getTexture("assets\\mapa\\podloga.png");
    floor->setTexture(flTexture);
    floor->setPosition(windowSize.x/2,windowSize.y/2);
    floor->setScale(1,1);
    set_proper_scale(floor, Scale_ratioX,Scale_ratioY);
    reset_origin_point(floor);
    to_draw.push_back(floor.get());
    ////////////////////////////////////////////////
    ///////////sciany///////////////////////
    Image image_sciany;
    image_sciany.loadFromFile("assets\\mapa\\sciany.png"); //potrzebne do kolizji (in progress)

    auto sciany = make_unique<Sprite>();
    Texture& scTexture = TextureManager::getInstance().getTexture("assets\\mapa\\sciany.png");
    sciany->setTexture(scTexture);
    sciany->setPosition(windowSize.x/2,windowSize.y/2);
    sciany->setScale(1,1);
    set_proper_scale(sciany, Scale_ratioX,Scale_ratioY);
    reset_origin_point(sciany);
    to_draw.push_back(sciany.get());
    ///////////////////////////////////
    ////////////////// bohater /////////////////////narazie orientacyjnie
    unique_ptr<bohater>hero = make_unique<bohater>();
    hero->setPosition(300,windowSize.y/2);
    hero->setScale(1.2f,1.2f);
    hero->set_v_ratio(1.f);
    hero->set_x_speed(120.f);
    hero->set_y_speed(120.f);
    hero->reset_origin_point();
    set_proper_scale(hero,Scale_ratioX,Scale_ratioY);
    postacie.push_back(hero.get());
    /////zasłona by bohater widział tylko to co ma widzieć/////
    //unique_ptr<Sprite> fog_of_war = make_unique<Sprite>();
    //Texture& fowTexture = TextureManager::getInstance().getTexture("assets\\bohater\\fog_of_war.png");
    //fog_of_war->setTexture(fowTexture);
    //reset_origin_point(fog_of_war);
    //fog_of_war->setPosition(hero->getPosition());
    //to_draw.push_back(fog_of_war.get());

    Shader fog_of_war;
    fog_of_war.loadFromFile("assets\\bohater\\mask.frag", Shader::Fragment);

    fog_of_war.setUniform("lightCenter", Glsl::Vec2(hero->getPosition()));
    fog_of_war.setUniform("lightRadius", 300.f);
    float aktualny_promien=300.f;
    RectangleShape mask(Vector2f(windowSize.x, windowSize.y));
    mask.setFillColor(Color::Black);
    window.draw(mask, &fog_of_war);
    ////////////////////////////////////////////////
    int frame_count=0, frame_count_h=0; //frame counter bohatera

    while (window.isOpen()) {
        Time elapsed=clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        ////////////ruszanie///////////

        move_hero(hero,elapsed,Scale_ratioX,Scale_ratioY,image_sciany);
        //fog_of_war->setPosition(hero->getPosition());
        fog_of_war.setUniform("lightCenter", hero->getPosition());
        fog_of_war.setUniform("lightRadius", aktualny_promien);
        if (frame_count%10+1>9){
            frame_count_h++;
        }
        hero->change_frame(frame_count_h);
        ///////////////////////////////
        ///////////DRAWING/////////////
        for (auto &d : to_draw){
            window.draw(*d);
        }
        for (auto &d : postacie){
            window.draw(*d);
        }
        window.draw(mask, &fog_of_war);
        window.display();
        frame_count++;
    }
=======
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class CustomRectangleShape : public sf::RectangleShape
{
private:
    bool is_selected = false;
    sf::Vector2f velocity;
    float minX, maxX, minY, maxY;
public:
    CustomRectangleShape(const sf::Vector2f& size, const sf::Vector2f& position): sf::RectangleShape(size)
    {
        setPosition(position);
        velocity = {0, 0};
        minX = minY = 0;
        maxX = maxY = 0;
    }
    void setSpeed(float vx, float vy)
    {
        velocity.x = vx;
        velocity.y = vy;
    }
    void setBounds(float left, float right, float top, float bottom)
    {
        minX = left;
        maxX = right - getSize().x;
        minY = top;
        maxY = bottom - getSize().y;
    }
    void select()
    {
        is_selected = true;
    }
    void unselect()
    {
        is_selected = false;
    }
    bool containsPoint(const sf::Vector2f& point) const
    {
        return getGlobalBounds().contains(point);
    }
    void animate(float dt)
    {
        if (!is_selected)
        {
            sf::Vector2f pos = getPosition();
            pos += velocity * dt;
            if (pos.x < minX)
            {
                pos.x = minX;
                velocity.x = -velocity.x;
            }
            else if (pos.x > maxX)
            {
                pos.x = maxX;
                velocity.x = -velocity.x;
            }
            if (pos.y < minY)
            {
                pos.y = minY;
                velocity.y = -velocity.y;
            }
            else if (pos.y > maxY)
            {
                pos.y = maxY;
                velocity.y = -velocity.y;
            }
            setPosition(pos);
        }
        else
        {
            sf::Vector2f pos = getPosition();
            float speed = 200;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                pos.x -= speed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                pos.x += speed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                pos.y -= speed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                pos.y += speed * dt;
            }
            if (pos.x < minX) pos.x = minX;
            if (pos.x > maxX) pos.x = maxX;
            if (pos.y < minY) pos.y = minY;
            if (pos.y > maxY) pos.y = maxY;
            setPosition(pos);
        }
    }
};

int main() {
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangles");
    window.setFramerateLimit(60);

    std::vector<CustomRectangleShape> rectangles;
    for (int i = 0; i < 10; ++i)
    {
        sf::Vector2f size(120, 60);
        sf::Vector2f pos
        (
            rand() % (window.getSize().x - static_cast<int>(size.x)),
            rand() % (window.getSize().y - static_cast<int>(size.y))
        );
        rectangles.emplace_back(size, pos);
        rectangles.back().setFillColor(sf::Color::Green);
        rectangles.back().setBounds(0, window.getSize().x, 0, window.getSize().y);
        float vx = (rand() % 100) + 50;
        float vy = (rand() % 100) + 50;
        rectangles.back().setSpeed(vx, vy);
    }
    if (!rectangles.empty())
    {
        rectangles[0].select();
        rectangles[0].setFillColor(sf::Color::Red);
    }
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f clickPos = window.mapPixelToCoords(
                    sf::Mouse::getPosition(window)
                );
                for (auto& rec : rectangles)
                {
                    if (rec.containsPoint(clickPos))
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            rec.select();
                            rec.setFillColor(sf::Color
                            (
                                std::rand() % 256,
                                std::rand() % 256,
                                std::rand() % 256
                            ));
                        }
                        else if (event.mouseButton.button == sf::Mouse::Right)
                        {
                            rec.unselect();
                            rec.setFillColor(sf::Color::Green);
                        }
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();
        window.clear(sf::Color::Black);
        for (auto& rec : rectangles) {
            rec.animate(dt);
            window.draw(rec);
        }
        window.display();
    }

>>>>>>> lokalne-zmiany
    return 0;
}
