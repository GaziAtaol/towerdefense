#include "Game.hpp"

#include "../ui/Menus.hpp"
#include "../ui/LevelSelect.hpp"
#include "../ui/SettingsPanel.hpp"
#include "../ui/Codex.hpp"
#include "../levels/Editor.hpp"
#include <iostream>
#include <map>

namespace core {

namespace {
ui::MenuScreen g_mainMenu;
ui::LevelSelect g_levelSelect;
ui::SettingsPanel g_settingsPanel;
ui::CodexView g_codex;
levels::LevelEditor g_editor;
}

Game::Game(ResourceManager& resources, const data::GameDatabase& database)
    : m_resources(resources), m_database(database) {
    m_settings = database.settings;
    m_save = database.save;
    m_hud.init(resources.font("default"));
    g_mainMenu = ui::MenuScreen{};
    ui::Button start(resources.font("default"), "Play", {420.f, 200.f});
    start.setCallback([this]() { setState(GameState::LevelSelect); });
    ui::Button settings(resources.font("default"), "Settings", {420.f, 260.f});
    settings.setCallback([this]() { setState(GameState::Settings); });
    ui::Button codexBtn(resources.font("default"), "Codex", {420.f, 320.f});
    codexBtn.setCallback([this]() { setState(GameState::Codex); });
    ui::Button editorBtn(resources.font("default"), "Level Editor", {420.f, 380.f});
    editorBtn.setCallback([this]() { setState(GameState::Editor); });
    g_mainMenu.addButton(start);
    g_mainMenu.addButton(settings);
    g_mainMenu.addButton(codexBtn);
    g_mainMenu.addButton(editorBtn);

    g_levelSelect.init(resources.font("default"));
    std::map<std::string, std::string> levels;
    for (const auto& [id, level] : database.levels) {
        levels[id] = level.name;
    }
    g_levelSelect.setLevels(levels);
    g_levelSelect.setOnSelect([this](const std::string& id) { startLevel(id); });

    g_settingsPanel.init(resources.font("default"), m_settings);
    g_codex.init(resources.font("default"));
    g_codex.setDatabase(database);
    g_editor.init(16, 12, 32, resources.font("default"));
}

void Game::setState(GameState state) {
    m_state = state;
    if (state == GameState::LevelSelect) {
        g_levelSelect.setOnSelect([this](const std::string& id) { startLevel(id); });
    }
}

void Game::handleEvent(const sf::Event& event, const sf::Vector2f& mouseWorld) {
    if (m_state == GameState::MainMenu) {
        if (event.type == sf::Event::MouseButtonPressed) {
            g_mainMenu.handleClick(mouseWorld);
        }
        return;
    }
    if (m_state == GameState::LevelSelect) {
        if (event.type == sf::Event::MouseButtonPressed) {
            g_levelSelect.handleClick(mouseWorld);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            setState(GameState::MainMenu);
        }
        return;
    }
    if (m_state == GameState::Settings) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            setState(GameState::MainMenu);
        }
        return;
    }
    if (m_state == GameState::Codex) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            setState(GameState::MainMenu);
        }
        return;
    }
    if (m_state == GameState::Editor) {
        g_editor.handleEvent(event);
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            setState(GameState::MainMenu);
        }
        return;
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            m_paused = !m_paused;
        }
        if (event.key.code == sf::Keyboard::Num1) m_speed = SpeedMode::Normal;
        if (event.key.code == sf::Keyboard::Num2) m_speed = SpeedMode::Double;
        if (event.key.code == sf::Keyboard::Num3) m_speed = SpeedMode::Triple;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        tryPlaceTower(mouseWorld);
    }
}

void Game::update(float dt) {
    if (m_state == GameState::Gameplay && !m_paused) {
        float speedMultiplier = static_cast<int>(m_speed);
        dt *= speedMultiplier;
        updateEconomy(dt);
        int livesLost = 0;
        systems::updateMovement(m_registry, m_paths, dt, static_cast<float>(m_currentLevel.definition.tileSize), livesLost);
        m_lives -= livesLost;
        if (m_lives <= 0) {
            m_state = GameState::Defeat;
            return;
        }

        m_grid.clear();
        m_enemyOrder.clear();
        for (const auto& [entity, stats] : m_registry.m_enemyStats) {
            m_enemyOrder.push_back(entity);
            m_grid.insert(m_enemyOrder.size() - 1, m_registry.m_transforms[entity].position);
        }

        systems::updateStatus(m_registry, dt, m_database.balance);
        systems::updateTargeting(m_registry, m_grid, m_enemyOrder, dt);
        systems::updateFiring(m_registry, m_projectilePool, dt, m_database.balance);
        systems::updateProjectiles(m_registry, dt, m_database.balance);
        systems::updateCleanup(m_registry, m_projectilePool, m_effectPool);

        if (!m_pendingSpawns.empty()) {
            m_spawnTimer -= dt;
            if (m_spawnTimer <= 0.f) {
                auto spawn = m_pendingSpawns.back();
                m_pendingSpawns.pop_back();
                spawnEnemyFromWave(spawn);
                if (!m_pendingSpawns.empty()) {
                    m_spawnTimer = spawn.delay;
                }
            }
        } else if (m_waveInProgress && m_registry.m_enemyStats.empty()) {
            m_waveInProgress = false;
            m_coins += static_cast<int>(m_database.balance.waveClearBonus);
            if (m_waveIndex >= static_cast<int>(m_database.waves[m_currentLevelId].waves.size())) {
                m_state = GameState::Victory;
            }
        }

        if (!m_waveInProgress) {
            m_waveTimer -= dt;
            if (m_waveTimer <= 0.f) {
                spawnWave();
            }
        }

        m_hud.update(m_lives, m_coins, m_waveIndex, static_cast<int>(m_speed), m_paused);
    } else if (m_state == GameState::Gameplay && m_paused) {
        m_hud.update(m_lives, m_coins, m_waveIndex, static_cast<int>(m_speed), m_paused);
    }
}

void Game::draw(sf::RenderWindow& window) {
    if (m_state == GameState::MainMenu) {
        g_mainMenu.draw(window);
        return;
    }
    if (m_state == GameState::LevelSelect) {
        g_levelSelect.draw(window);
        return;
    }
    if (m_state == GameState::Settings) {
        g_settingsPanel.draw(window);
        return;
    }
    if (m_state == GameState::Codex) {
        g_codex.draw(window);
        return;
    }
    if (m_state == GameState::Editor) {
        g_editor.draw(window);
        return;
    }
    if (m_state == GameState::Gameplay || m_state == GameState::Paused || m_state == GameState::Victory || m_state == GameState::Defeat) {
        m_tilemap.draw(window);
        for (const auto& [entity, render] : m_registry.m_renderables) {
            auto transformIt = m_registry.m_transforms.find(entity);
            if (transformIt == m_registry.m_transforms.end()) continue;
            sf::CircleShape shape(12.f);
            shape.setPosition(transformIt->second.position - sf::Vector2f{12.f, 12.f});
            if (m_registry.m_enemyStats.count(entity)) {
                shape.setFillColor(sf::Color::Red);
            } else if (m_registry.m_towerStats.count(entity)) {
                shape.setFillColor(sf::Color::Blue);
            } else if (m_registry.m_projectiles.count(entity)) {
                shape.setFillColor(sf::Color::Yellow);
                shape.setRadius(6.f);
            }
            window.draw(shape);
        }
        m_hud.draw(window);
        if (m_state == GameState::Victory) {
            sf::Text text("Victory!", m_resources.font("default"), 32);
            text.setPosition(420.f, 200.f);
            window.draw(text);
        }
        if (m_state == GameState::Defeat) {
            sf::Text text("Defeat", m_resources.font("default"), 32);
            text.setPosition(420.f, 200.f);
            window.draw(text);
        }
    }
}

void Game::startLevel(const std::string& id) {
    auto it = m_database.levels.find(id);
    if (it == m_database.levels.end()) return;
    m_currentLevelId = id;
    m_currentLevel = levels::buildLevel(it->second);
    m_tilemap.build(m_currentLevel, m_resources.texture("tiles"));
    m_paths.paths = m_currentLevel.paths;
    m_registry = ecs::Registry{};
    m_projectilePool.available.clear();
    m_effectPool.available.clear();
    m_enemyOrder.clear();
    m_lives = it->second.startLives;
    m_coins = it->second.startCoins;
    m_waveIndex = 0;
    m_waveTimer = 2.f;
    m_waveInProgress = false;
    m_paused = false;
    m_state = GameState::Gameplay;
}

void Game::spawnWave() {
    auto waveDataIt = m_database.waves.find(m_currentLevelId);
    if (waveDataIt == m_database.waves.end()) return;
    if (m_waveIndex >= static_cast<int>(waveDataIt->second.waves.size())) return;
    const auto& wave = waveDataIt->second.waves[m_waveIndex];
    m_pendingSpawns.clear();
    for (const auto& spawn : wave.enemies) {
        m_pendingSpawns.push_back(spawn);
    }
    std::reverse(m_pendingSpawns.begin(), m_pendingSpawns.end());
    if (!m_pendingSpawns.empty()) {
        m_spawnTimer = m_pendingSpawns.back().delay;
    }
    m_waveIndex++;
    m_waveInProgress = true;
    m_waveTimer = wave.spawnInterval;
}

void Game::spawnEnemyFromWave(const data::WaveSpawn& spawn) {
    auto enemyDefIt = m_database.enemies.find(spawn.type);
    if (enemyDefIt == m_database.enemies.end()) return;
    for (int i = 0; i < spawn.count; ++i) {
        const auto& path = m_paths.paths[i % std::max<std::size_t>(1, m_paths.paths.size())];
        entities::spawnEnemy(m_registry, enemyDefIt->second, path);
    }
}

void Game::updateEconomy(float dt) {
    m_incomeTimer += dt;
    if (m_incomeTimer >= 1.f) {
        int totalIncome = 0;
        for (const auto& [entity, eco] : m_registry.m_economy) {
            totalIncome += static_cast<int>(eco.income);
        }
        m_coins += totalIncome;
        m_incomeTimer = 0.f;
    }
}

void Game::tryPlaceTower(const sf::Vector2f& position) {
    for (const auto& cell : m_currentLevel.definition.buildable) {
        sf::Vector2f cellCenter = {cell.x * static_cast<float>(m_currentLevel.definition.tileSize) + m_currentLevel.definition.tileSize * 0.5f,
                                   cell.y * static_cast<float>(m_currentLevel.definition.tileSize) + m_currentLevel.definition.tileSize * 0.5f};
        if (math::distance(cellCenter, position) < m_currentLevel.definition.tileSize * 0.5f) {
            auto towerIt = m_database.towers.find("arrow_mk1");
            if (towerIt == m_database.towers.end()) return;
            if (m_coins < towerIt->second.cost) return;
            m_coins -= towerIt->second.cost;
            entities::spawnTower(m_registry, towerIt->second, cellCenter);
            break;
        }
    }
}

} // namespace core

