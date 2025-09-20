#include "App.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace {

std::filesystem::path findProjectRoot() {
    namespace fs = std::filesystem;
    auto current = fs::current_path();
    for (int depth = 0; depth < 10; ++depth) {
        if (fs::exists(current / "data/settings.json")) {
            return current;
        }
        if (!current.has_parent_path()) {
            break;
        }
        current = current.parent_path();
    }
    throw std::runtime_error("TowerDefense project root could not be located. Ensure the data directory is available.");
}

std::filesystem::path resolveDataPath(const std::filesystem::path& projectRoot) {
    namespace fs = std::filesystem;
    if (const char* env = std::getenv("TOWERDEFENSE_DATA")) {
        fs::path custom(env);
        if (fs::exists(custom)) {
            return custom;
        }
    }
    auto dataPath = projectRoot / "data";
    if (!fs::exists(dataPath)) {
        throw std::runtime_error("Data directory not found. Expected at: " + dataPath.string());
    }
    return dataPath;
}

std::filesystem::path resolveAssetsPath(const std::filesystem::path& projectRoot) {
    namespace fs = std::filesystem;
    if (const char* env = std::getenv("TOWERDEFENSE_ASSETS")) {
        fs::path custom(env);
        return custom;
    }
    return projectRoot / "assets";
}

} // namespace

namespace core {

App::App() {
    m_window.create(sf::VideoMode(1280, 720), "TowerDefense", sf::Style::Default);
    m_window.setFramerateLimit(60);
    m_projectRoot = findProjectRoot();
    m_dataPath = resolveDataPath(m_projectRoot);
    m_assetsPath = resolveAssetsPath(m_projectRoot);
    if (!std::filesystem::exists(m_assetsPath)) {
        std::cerr << "[App] Asset directory could not be found at '" << m_assetsPath
                  << "'. Procedural placeholders will be used.\n";
    }
    m_resources.setAssetRoot(m_assetsPath);
    m_resources.loadTexture("tiles", "textures/placeholder.png");
    m_resources.loadTexture("ui", "textures/placeholder.png");
    m_resources.loadSound("click", "audio/placeholder.wav");
    m_resources.loadFont("default", "fonts/DejaVuSans.ttf");
    m_database = m_loader.loadAll(m_dataPath.string());
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

