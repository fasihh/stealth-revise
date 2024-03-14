#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>

#include "gameState.hpp"

// class

class Game {
private:
    sf::RenderWindow *window;
    sf::Event sfEvent;

    sf::Clock dtClock;
    float dt;

    std::stack<State *> states;

    void initWindow();
    void initStates();
public:
    Game();
    virtual ~Game();

    // game functions

    void endApplication();

    // update
    void updateDt();
    void updateSFMLEvents();
    void update();

    // render
    void render();

    // main
    void run();
};


// impl

// private functions

void Game::initWindow() {
    std::ifstream ifs("config/window.ini");

    // default
    std::string title = "None";
    sf::VideoMode window_bounds(800, 600);
    unsigned frame_limit = 60, antialiasing = 0;
    bool vertical_sync = false;

    if (ifs.is_open()) {
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs >> frame_limit;
        ifs >> vertical_sync;
        ifs >> antialiasing;
    }
    ifs.close();

    this->window = new sf::RenderWindow{
        window_bounds,
        title,
        sf::Style::Default,
        sf::ContextSettings(0, 0, antialiasing)
    };
    this->window->setFramerateLimit(frame_limit);
    this->window->setVerticalSyncEnabled(vertical_sync);
}

void Game::initStates() {
    this->states.push(new GameState(this->window));
}

// constructors/destructors

Game::Game() {
    this->initWindow();
    this->initStates();
}

Game::~Game() {
    delete this->window;

    while(!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }
}

// public functions

void Game::endApplication() {
    std::cout << "Ending Game" << std::endl;
    this->window->close();
}

void Game::updateDt() {
    // updates dt with update+render time of a frame
    this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSFMLEvents() {
    while(this->window->pollEvent(this->sfEvent)) {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}

void Game::update() {
    this->updateSFMLEvents();

    // update top state
    if (!this->states.empty()) {
        this->states.top()->update(this->dt);

        // check if requested end status
        if (this->states.top()->getEndStatus()) {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }

        return;
    }

    this->endApplication();
}

void Game::render() {
    this->window->clear(); // clear screen

    // render top state
    if (!this->states.empty())
        this->states.top()->render(this->window);

    this->window->display();
}

void Game::run() {
    while (this->window->isOpen()) {
        this->updateDt(); // update the clock
        this->update(); // update frame
        this->render(); // display frame
    }
}
