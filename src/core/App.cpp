#include "App.hpp"

#include <filesystem>
#include <iostream>

namespace core {

App::App() {
    m_window.create(sf::VideoMode(1280, 720), "TowerDefense", sf::Style::Default);
    m_window.setFramerateLimit(60);
    std::filesystem::create_directory("data");
    m_resources.loadTexture("tiles", "assets/textures/placeholder.png");
    m_resources.loadTexture("ui", "assets/textures/placeholder.png");
    m_resources.loadSound("click", "assets/audio/placeholder.wav");
    m_resources.loadFont("default", "assets/fonts/DejaVuSans.ttf");
    m_database = m_loader.loadAll("data");
    m_game = std::make_unique<Game>(m_resources, m_database);
}

int App::run() {
    while (m_window.isOpen()) {
        processEvents();
        float dt = m_time.tick();
        update(dt);
        render();
    }
    return 0;
}

void App::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        sf::Vector2f mouseWorld = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
        m_game->handleEvent(event, mouseWorld);
    }
}

void App::update(float dt) {
    m_game->update(dt);
}

void App::render() {
    m_window.clear(sf::Color(20, 30, 30));
    m_game->draw(m_window);
    m_window.display();
}

} // namespace core

