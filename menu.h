#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

std::vector<sf::Time> loadScores();
void saveScore(sf::Time time);

enum GameState {
    MENU,
    PLAYING,
    EXITING
};

class Menu {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Font scoreFont;
    std::array<sf::Text, 2> options;
    int selectedItem = 0;
    std::vector<sf::Text> scoreTexts;
    std::vector<sf::Time> bestScores;

public:
    Menu(sf::RenderWindow* win) : window(win)
    {
        if (!font.loadFromFile("assets/font.ttf"))
        {
            std::cerr << "Blad: nie udalo sie zaladowac glownej czcionki!" << std::endl;
        }
        if (!scoreFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        {
            std::cerr << "Blad: nie udalo sie zaladowac czcionki systemowej do wynikow!" << std::endl;
        }
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

        loadBestScores();
    }

    void draw()
    {
        window->clear();
        for (auto& option : options)
        {
            window->draw(option);
        }
        for (auto& scoreText : scoreTexts)
        {
            window->draw(scoreText);
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
    void loadBestScores()
    {
        bestScores = loadScores();
        scoreTexts.clear();

        sf::Text header;
        header.setFont(font);
        header.setString("Najlepsze wyniki:");
        header.setCharacterSize(48);
        header.setPosition(600, 150);
        header.setFillColor(sf::Color::Yellow);
        scoreTexts.push_back(header);

        for (size_t i = 0; i < std::min(bestScores.size(), size_t(5)); ++i)
        {
            int minutes = static_cast<int>(bestScores[i].asSeconds()) / 60;
            int seconds = static_cast<int>(bestScores[i].asSeconds()) % 60;


            std::ostringstream ss;
            ss << (i + 1) << ". "<< minutes << ":"<< std::setw(2) << std::setfill('0') << seconds;

            sf::Text scoreText;
            scoreText.setFont(scoreFont);
            scoreText.setString(ss.str());
            scoreText.setCharacterSize(36);
            scoreText.setPosition(600, 220 + i * 50);
            scoreText.setFillColor(sf::Color::White);
            scoreTexts.push_back(scoreText);
        }
    }
private:
    void updateSelection()
    {
        for (size_t i = 0; i < options.size(); ++i)
        {
            options[i].setFillColor(i == selectedItem ? sf::Color::Red : sf::Color::White);
        }
    }
};

#endif // MENU_H
