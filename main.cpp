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

    return 0;
}
