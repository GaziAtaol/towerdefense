#pragma once

#include "DataLoader.hpp"
#include "GameData.hpp"
#include "ResourceManager.hpp"
#include "../ecs/Registry.hpp"
#include "../systems/Systems.hpp"
#include "../entities/Entities.hpp"
#include "../levels/LevelLoader.hpp"
#include "../levels/Tilemap.hpp"
#include "../math/SpatialHash.hpp"
#include "../ui/HUD.hpp"
#include <SFML/Graphics.hpp>

namespace core {

enum class GameState { MainMenu, LevelSelect, Settings, Codex, Gameplay, Paused, Victory, Defeat, Editor };

enum class SpeedMode { Normal = 1, Double = 2, Triple = 3 };

class Game {
public:
    Game(ResourceManager& resources, const data::GameDatabase& database);

    void handleEvent(const sf::Event& event, const sf::Vector2f& mouseWorld);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    GameState state() const { return m_state; }
    void setState(GameState state);

private:
    void startLevel(const std::string& id);
    void updateMenus();
    void spawnWave();
    void spawnEnemyFromWave(const data::WaveSpawn& spawn);
    void updateEconomy(float dt);
    void tryPlaceTower(const sf::Vector2f& position);

    ResourceManager& m_resources;
    const data::GameDatabase& m_database;

    GameState m_state = GameState::MainMenu;
    SpeedMode m_speed = SpeedMode::Normal;

    data::SettingsData m_settings;
    data::SaveData m_save;

    levels::LevelRuntime m_currentLevel;
    levels::TilemapRenderer m_tilemap;

    ecs::Registry m_registry;
    systems::PathContext m_paths;
    systems::ProjectilePool m_projectilePool;
    systems::EffectPool m_effectPool;
    math::SpatialHashGrid m_grid;
    std::vector<ecs::Entity> m_enemyOrder;

    ui::HUD m_hud;

    int m_lives = 20;
    int m_coins = 0;
    int m_waveIndex = 0;
    float m_waveTimer = 0.f;
    bool m_waveInProgress = false;
    std::vector<data::WaveSpawn> m_pendingSpawns;
    float m_spawnTimer = 0.f;
    std::string m_currentLevelId;
    bool m_paused = false;
    float m_incomeTimer = 0.f;
};

} // namespace core

