#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <array>

enum GameState {
    MENU,
    PLAYING,
    EXITING
};

class Menu {
private:
    sf::RenderWindow* window;
    sf::Font font;
    std::array<sf::Text, 2> options;
    int selectedItem = 0;

public:
    Menu(sf::RenderWindow* win) : window(win) {
        font.loadFromFile("assets/font.ttf");

        options[0].setFont(font);
        options[0].setString("Graj");
        options[0].setCharacterSize(64);
        options[0].setPosition(200, 200);
        options[0].setFillColor(sf::Color::Red);

        options[1].setFont(font);
        options[1].setString("Wyjdz");
        options[1].setCharacterSize(64);
        options[1].setPosition(200, 300);
        options[1].setFillColor(sf::Color::White);
    }

    void draw() {
        window->clear();
        for (auto& option : options) {
            window->draw(option);
        }
        window->display();
    }
    int getSelectedItem() const
    {
        return selectedItem + 1; // 1 = Graj, 2 = Wyjdz
    }
    void handleInput(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                selectedItem = (selectedItem - 1 + options.size()) % options.size();
                updateSelection();
            } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                selectedItem = (selectedItem + 1) % options.size();
                updateSelection();
            }
        }
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            for (size_t i = 0; i < options.size(); ++i) {
                if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    selectedItem = i;
                    updateSelection();
                }
            }
        }

        // Klikniêcie myszy – ustawia wybran¹ opcjê
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f clickPos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            for (size_t i = 0; i < options.size(); ++i) {
                if (options[i].getGlobalBounds().contains(clickPos)) {
                    selectedItem = i;
                    updateSelection();
                }
            }
        }
    }
    const sf::Text& getOption(size_t index) const
    {
        return options.at(index);
    }
private:
    void updateSelection() {
        for (size_t i = 0; i < options.size(); ++i) {
            options[i].setFillColor(i == selectedItem ? sf::Color::Red : sf::Color::White);
        }
    }
};

#endif // MENU_H
