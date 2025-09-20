#pragma once

#include "DataLoader.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "TimeStep.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace core {

class App {
public:
    App();
    int run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow m_window;
    core::ResourceManager m_resources;
    core::DataLoader m_loader;
    data::GameDatabase m_database;
    std::unique_ptr<core::Game> m_game;
    core::TimeStep m_time;
};

} // namespace core

