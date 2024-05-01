#pragma once
#include "state.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "object.hpp"

// impl of state interface

class GameState : public State {
private:
    std::vector<Entity *> entities;
    std::vector<Object> objects;

    void initObjects();
    void initGameState();
public:
    GameState(sf::RenderWindow* window);
    virtual ~GameState();

    void endState();

    void updateKeyBinds(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target = nullptr);
};

void GameState::initObjects() {
    // //Middle Chowrangi
    Object Chowrangi(sf::Vector2f(80.f,60.f),sf::Color::White);
    Chowrangi.setPosition(sf::Vector2f(440.f,330.f));
    // //Middle Chowrangi End

    // //Starting Area
    Object l1(sf::Vector2f(270.f,10.f));
    l1.setPosition(sf::Vector2f(4.f,60.f));
    Object sh1(sf::Vector2f(100.f,100.f));
    sh1.setPosition(sf::Vector2f(350.f,4.f));
    Object l2(sf::Vector2f(10.f,120.f));
    l2.setPosition(sf::Vector2f(274.f,60.f));
    Object l3(sf::Vector2f(180.f,10.f));
    l3.setPosition(sf::Vector2f(274.f,180.f));
    Object l4(sf::Vector2f(10.f,120.f));
    l4.setPosition(sf::Vector2f(520.f,64.f));
    Object l5(sf::Vector2f(226.f,10.f));
    l5.setPosition(sf::Vector2f(520.f,58.f));
    Object l6(sf::Vector2f(212.f,10.f));
    l6.setPosition(sf::Vector2f(584.f,130.f));
    Object l7(sf::Vector2f(10.f,74.f));
    l7.setPosition(sf::Vector2f(584.f,130.f));
    Object l8(sf::Vector2f(74.f,10.f));
    l8.setPosition(sf::Vector2f(520.f,184.f));
    // //Starting Area End

    // //East
    Object l9(sf::Vector2f(10.f,300.f));
    l9.setPosition(sf::Vector2f(274.f,149.f));
    Object l14(sf::Vector2f(10.f,56.f));
    l14.setPosition(sf::Vector2f(274.f,540.f));
    Object l10(sf::Vector2f(10.f,56.f));
    l10.setPosition(sf::Vector2f(182.f,140.f));
    Object l15(sf::Vector2f(10.f,276.f));
    l15.setPosition(sf::Vector2f(182.f,256.f));
    Object l11(sf::Vector2f(10.f,198.f));
    l11.setPosition(sf::Vector2f(91.f,140.f));
    Object l16(sf::Vector2f(10.f,198.f));
    l16.setPosition(sf::Vector2f(91.f,398.f));
    Object l12(sf::Vector2f(92.f,10.f));
    l12.setPosition(sf::Vector2f(182.f,140.f));
    Object l13(sf::Vector2f(91.f,10.f));
    l13.setPosition(sf::Vector2f(4.f,140.f));
    // //East End

    // //West
    Object l17(sf::Vector2f(10.f,402.f));
    l17.setPosition(sf::Vector2f(584.f,194.f));
    // //West End

    sf::Vector2f windowOutwardsPadding = sf::Vector2f{ 0.f, 0.f };
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(this->window->getSize());
    Object window(windowSize, sf::Color::Transparent);
    window.setPosition(-0.5f * windowOutwardsPadding);

    this->objects.push_back(l2);
    this->objects.push_back(l3);
    this->objects.push_back(l4);
    this->objects.push_back(l5);
    this->objects.push_back(l6);
    this->objects.push_back(l7);
    this->objects.push_back(l8);
    this->objects.push_back(l9);
    this->objects.push_back(l10);
    this->objects.push_back(l11);
    this->objects.push_back(l12);
    this->objects.push_back(l13);
    this->objects.push_back(l14);
    this->objects.push_back(l15);
    this->objects.push_back(l16);
    this->objects.push_back(l17);
    this->objects.push_back(l1);
    this->objects.push_back(Chowrangi);
    this->objects.push_back(sh1);
    this->objects.push_back(window);
}

void GameState::initGameState() {
    Entity *player = new Player(20.f, 120.f);
    player->setFillColor(sf::Color::Red);
    
    Entity *enemy1 = new Enemy(20.f, 100.f, player);
    enemy1->setPosition(300.f, 300.f);
    enemy1->setFillColor(sf::Color::Green);

    this->entities.push_back(enemy1);
    this->entities.push_back(player);
}

GameState::GameState(sf::RenderWindow* window) :
    State(window)
{
    this->initObjects();
    this->initGameState();
}

GameState::~GameState() {
    for (int i = 0; i < entities.size(); i++)
        delete entities[i];
}

void GameState::endState() {
    std::cout << "Game Over" << std::endl;
}

void GameState::updateKeyBinds(const float& dt) {
    this->checkEndStatus();
}

void GameState::update(const float& dt) {
    this->updateKeyBinds(dt);
    for (Entity *entity : this->entities) {
        
        entity->update(dt, this->objects);
    }
}

void GameState::render(sf::RenderTarget* target) {
    for (Entity *entity : this->entities)
        entity->render(target);
    for (Object& object : this->objects)
        object.render(target);
}