#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace data {

struct UpgradeModifier {
    int level = 0;
    float damage = 0.f;
    float fireRate = 0.f;
    float range = 0.f;
    float armorPen = 0.f;
    float aoeRadius = 0.f;
    float statusPotency = 0.f;
    float statusDuration = 0.f;
    float statusStacks = 0.f;
    float chain = 0.f;
    float pierce = 0.f;
    float income = 0.f;
};

struct BranchModifier {
    std::string name;
    std::string description;
    float damage = 0.f;
    float fireRate = 0.f;
    float range = 0.f;
    float armorPen = 0.f;
    float aoeRadius = 0.f;
    float chain = 0.f;
    float pierce = 0.f;
    float critChance = 0.f;
    float critMultiplier = 0.f;
    float statusPotency = 0.f;
    float statusDuration = 0.f;
    float income = 0.f;
    float magicResistShred = 0.f;
    bool canHitFlying = false;
};

struct TowerDefinition {
    std::string id;
    std::string name;
    std::string role;
    int cost = 0;
    float damage = 0.f;
    float fireRate = 1.f;
    float range = 100.f;
    float aoeRadius = 0.f;
    float armorPen = 0.f;
    float projectileSpeed = 280.f;
    float chain = 0.f;
    float pierce = 0.f;
    float statusPotency = 0.f;
    float statusDuration = 0.f;
    float income = 0.f;
    std::string projectileType;
    std::string statusEffect;
    bool canHitFlying = false;
    std::vector<std::string> tags;
    std::vector<UpgradeModifier> upgrades;
    BranchModifier branchA;
    BranchModifier branchB;
};

struct EnemyDefinition {
    std::string id;
    std::string name;
    float hp = 100.f;
    float speed = 60.f;
    float armor = 0.f;
    float magicResist = 0.f;
    int reward = 5;
    std::vector<std::string> abilities;
    std::vector<std::string> tags;
};

struct StatusDefinition {
    float multiplier = 1.f;
    float duration = 0.f;
    float dps = 0.f;
    float stackDps = 0.f;
    float stun = 0.f;
    float armor = 0.f;
    float magicResist = 0.f;
};

struct WaveSpawn {
    std::string type;
    int count = 0;
    float delay = 0.f;
};

struct WaveDefinition {
    int id = 0;
    float spawnInterval = 1.f;
    std::vector<WaveSpawn> enemies;
};

struct LevelWaves {
    std::vector<WaveDefinition> waves;
};

struct LevelDefinition {
    std::string id;
    std::string name;
    std::string biome;
    int width = 32;
    int height = 18;
    int tileSize = 40;
    std::vector<std::vector<int>> paths;
    std::vector<sf::Vector2i> buildable;
    std::vector<sf::Vector2i> obstacles;
    std::vector<std::string> rules;
    std::string wavesId;
    int startCoins = 300;
    int startLives = 20;
};

struct BalanceDefinition {
    int baseLives = 20;
    int baseCoins = 300;
    float enemyHpMultiplier = 1.f;
    float enemySpeedMultiplier = 1.f;
    float enemyRewardMultiplier = 1.f;
    std::unordered_map<int, sf::Vector3f> difficultyCurve; // x=hp,y=speed,z=reward multipliers
    std::unordered_map<std::string, StatusDefinition> statuses;
    float killRewardBonus = 0.f;
    float waveClearBonus = 0.f;
    float sellRefund = 0.5f;
};

struct SettingsData {
    float audioVolume = 1.f;
    float musicVolume = 1.f;
    int gameSpeed = 1;
    std::string graphicsQuality = "medium";
    std::string colorBlindMode = "normal";
};

struct SaveData {
    int lastUnlockedLevel = 1;
    int coins = 0;
    std::vector<std::string> badges;
    std::vector<std::string> unlockedTowers;
    std::vector<std::string> completedLevels;
};

struct GameDatabase {
    std::unordered_map<std::string, TowerDefinition> towers;
    std::unordered_map<std::string, EnemyDefinition> enemies;
    std::unordered_map<std::string, LevelWaves> waves;
    std::unordered_map<std::string, LevelDefinition> levels;
    BalanceDefinition balance;
    SettingsData settings;
    SaveData save;
};

} // namespace data

