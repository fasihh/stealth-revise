#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stack>
#include <map>

// asbtract state

class State {
private:
protected:
    sf::RenderWindow* window;
    std::vector<sf::Texture *> textures;
    bool endStatus;
public:
    State(sf::RenderWindow* window);
    virtual ~State();

    const bool& getEndStatus() const;
    const sf::RenderWindow& getWindow() const;

    virtual void checkEndStatus();
    virtual void endState() = 0;

    virtual void updateKeyBinds(const float& dt) = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

State::State(sf::RenderWindow* window) :
    window(window), endStatus(false) 
{
}

State::~State() {
    for (int i = 0; i < textures.size(); i++)
        delete textures[i];
}

void State::checkEndStatus() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->endStatus = true;
}

const bool& State::getEndStatus() const {
    return this->endStatus;
}