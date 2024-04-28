#pragma once
#include "state.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "object.hpp"

// impl of state interface

class GameState :
    public State
{
private:
    std::vector<Entity *> entities;
    std::vector<Object> objects;

    void initGameState();
public:
    GameState(sf::RenderWindow* window);
    virtual ~GameState();

    void endState();

    void updateKeyBinds(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target = nullptr);
};

void GameState::initGameState() {
    Object object1(sf::Vector2f{ 100.f, 300.f }, sf::Color::Green);
    object1.setPosition(sf::Vector2f{ 100.f, 100.f });

    sf::Vector2f windowOutwardsPadding = sf::Vector2f{ /*10.f, 10.f*/ 0.f, 0.f };
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(this->window->getSize());
    Object window(windowSize /* + windowOutwardsPadding */, sf::Color::Transparent);
    window.setPosition(-0.5f * windowOutwardsPadding);

    this->objects.push_back(object1);
    this->objects.push_back(window);

    Entity *player = new Player(20.f, 120.f);
    player->setFillColor(sf::Color::Red);
    Entity *enemy = new Enemy(20.f, 100.f, player);
    enemy->setPosition(300.f, 300.f);
    enemy->setFillColor(sf::Color::Green);

    this->entities.push_back(enemy);
    this->entities.push_back(player);
}

GameState::GameState(sf::RenderWindow* window) :
    State(window)
{
   this->initGameState();
}

GameState::~GameState() {
    for (int i = 0; i < entities.size(); i++)
        delete entities[i];
}

void GameState::endState() {
    std::cout << "Ending GameState" << std::endl;
}

void GameState::updateKeyBinds(const float& dt) {
    this->checkEndStatus();
}

void GameState::update(const float& dt) {
    this->updateKeyBinds(dt);
    for (Entity *entity : this->entities)
        entity->update(dt, this->objects);
}

void GameState::render(sf::RenderTarget* target) {
    for (Entity *entity : this->entities)
        entity->render(target);
    for (Object& object : this->objects)
        object.render(target);
}