#include "DataLoader.hpp"

#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace core {

using nlohmann::json;

namespace {

json loadJsonFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + path);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return json::parse(content);
}

std::vector<sf::Vector2i> parseVector2iArray(const json& arr) {
    std::vector<sf::Vector2i> out;
    for (const auto& value : arr) {
        if (value.is_array() && value.size() >= 2) {
            out.emplace_back(static_cast<int>(value[0].get<float>()), static_cast<int>(value[1].get<float>()));
        }
    }
    return out;
}

std::vector<std::vector<int>> parsePaths(const json& pathArray) {
    std::vector<std::vector<int>> out;
    for (const auto& path : pathArray) {
        const auto& points = path.at("points");
        std::vector<int> flat;
        for (const auto& point : points) {
            flat.push_back(static_cast<int>(point[0].get<float>()));
            flat.push_back(static_cast<int>(point[1].get<float>()));
        }
        out.emplace_back(std::move(flat));
    }
    return out;
}

void applyBranch(const json& branchJson, data::BranchModifier& branch) {
    if (branchJson.contains("name")) branch.name = branchJson.at("name").get<std::string>();
    if (branchJson.contains("description")) branch.description = branchJson.at("description").get<std::string>();
    if (branchJson.contains("damage")) branch.damage = branchJson.at("damage").get<float>();
    if (branchJson.contains("fireRate")) branch.fireRate = branchJson.at("fireRate").get<float>();
    if (branchJson.contains("range")) branch.range = branchJson.at("range").get<float>();
    if (branchJson.contains("armorPen")) branch.armorPen = branchJson.at("armorPen").get<float>();
    if (branchJson.contains("aoeRadius")) branch.aoeRadius = branchJson.at("aoeRadius").get<float>();
    if (branchJson.contains("chain")) branch.chain = branchJson.at("chain").get<float>();
    if (branchJson.contains("pierce")) branch.pierce = branchJson.at("pierce").get<float>();
    if (branchJson.contains("critChance")) branch.critChance = branchJson.at("critChance").get<float>();
    if (branchJson.contains("critMultiplier")) branch.critMultiplier = branchJson.at("critMultiplier").get<float>();
    if (branchJson.contains("statusPotency")) branch.statusPotency = branchJson.at("statusPotency").get<float>();
    if (branchJson.contains("statusDuration")) branch.statusDuration = branchJson.at("statusDuration").get<float>();
    if (branchJson.contains("income")) branch.income = branchJson.at("income").get<float>();
    if (branchJson.contains("magicResistShred")) branch.magicResistShred = branchJson.at("magicResistShred").get<float>();
    if (branchJson.contains("canHitFlying")) branch.canHitFlying = branchJson.at("canHitFlying").get<bool>();
}

} // namespace

data::GameDatabase DataLoader::loadAll(const std::string& dataPath) {
    data::GameDatabase db;

    // Towers
    auto towersJson = loadJsonFile(dataPath + "/towers.json");
    for (const auto& tower : towersJson.at("towers")) {
        data::TowerDefinition def;
        def.id = tower.at("id").get<std::string>();
        def.name = tower.at("name").get<std::string>();
        def.role = tower.at("role").get<std::string>();
        def.cost = static_cast<int>(tower.at("cost").get<float>());
        def.damage = tower.at("damage").get<float>();
        def.fireRate = tower.at("fireRate").get<float>();
        def.range = tower.at("range").get<float>();
        def.projectileType = tower.at("projectileType").get<std::string>();
        if (tower.contains("aoeRadius")) def.aoeRadius = tower.at("aoeRadius").get<float>();
        if (tower.contains("armorPen")) def.armorPen = tower.at("armorPen").get<float>();
        if (tower.contains("chain")) def.chain = tower.at("chain").get<float>();
        if (tower.contains("pierce")) def.pierce = tower.at("pierce").get<float>();
        if (tower.contains("statusEffect")) def.statusEffect = tower.at("statusEffect").get<std::string>();
        if (tower.contains("statusPotency")) def.statusPotency = tower.at("statusPotency").get<float>();
        if (tower.contains("statusDuration")) def.statusDuration = tower.at("statusDuration").get<float>();
        if (tower.contains("income")) def.income = tower.at("income").get<float>();
        def.canHitFlying = tower.value("canHitFlying", false);
        if (tower.contains("tags")) {
            for (const auto& tag : tower.at("tags")) {
                def.tags.push_back(tag.get<std::string>());
            }
        }
        if (tower.contains("upgrades")) {
            for (const auto& upgrade : tower.at("upgrades")) {
                data::UpgradeModifier mod;
                mod.level = static_cast<int>(upgrade.at("lvl").get<float>());
                if (upgrade.contains("damage")) mod.damage = upgrade.at("damage").get<float>();
                if (upgrade.contains("fireRate")) mod.fireRate = upgrade.at("fireRate").get<float>();
                if (upgrade.contains("range")) mod.range = upgrade.at("range").get<float>();
                if (upgrade.contains("armorPen")) mod.armorPen = upgrade.at("armorPen").get<float>();
                if (upgrade.contains("aoeRadius")) mod.aoeRadius = upgrade.at("aoeRadius").get<float>();
                if (upgrade.contains("statusPotency")) mod.statusPotency = upgrade.at("statusPotency").get<float>();
                if (upgrade.contains("statusDuration")) mod.statusDuration = upgrade.at("statusDuration").get<float>();
                if (upgrade.contains("statusStacks")) mod.statusStacks = upgrade.at("statusStacks").get<float>();
                if (upgrade.contains("chain")) mod.chain = upgrade.at("chain").get<float>();
                if (upgrade.contains("pierce")) mod.pierce = upgrade.at("pierce").get<float>();
                if (upgrade.contains("income")) mod.income = upgrade.at("income").get<float>();
                def.upgrades.push_back(mod);
            }
        }
        if (tower.contains("branchA")) applyBranch(tower.at("branchA"), def.branchA);
        if (tower.contains("branchB")) applyBranch(tower.at("branchB"), def.branchB);
        db.towers[def.id] = def;
    }

    // Enemies
    auto enemiesJson = loadJsonFile(dataPath + "/enemies.json");
    for (const auto& enemy : enemiesJson.at("enemies")) {
        data::EnemyDefinition def;
        def.id = enemy.at("id").get<std::string>();
        def.name = enemy.at("name").get<std::string>();
        def.hp = enemy.at("hp").get<float>();
        def.speed = enemy.at("speed").get<float>();
        def.armor = enemy.at("armor").get<float>();
        def.magicResist = enemy.at("magicResist").get<float>();
        def.reward = static_cast<int>(enemy.at("reward").get<float>());
        for (const auto& ability : enemy.at("abilities")) {
            def.abilities.push_back(ability.get<std::string>());
        }
        for (const auto& tag : enemy.at("tags")) {
            def.tags.push_back(tag.get<std::string>());
        }
        db.enemies[def.id] = def;
    }

    // Waves
    auto wavesJson = loadJsonFile(dataPath + "/waves.json");
    const auto& levels = wavesJson.at("levels");
    for (const auto& [levelId, value] : levels.get<nlohmann::json::object_t>()) {
        data::LevelWaves lw;
        for (const auto& wave : value.at("waves")) {
            data::WaveDefinition def;
            def.id = static_cast<int>(wave.at("id").get<float>());
            def.spawnInterval = wave.at("spawnInterval").get<float>();
            for (const auto& enemy : wave.at("enemies")) {
                data::WaveSpawn spawn;
                spawn.type = enemy.at("type").get<std::string>();
                spawn.count = static_cast<int>(enemy.at("count").get<float>());
                if (enemy.contains("delay")) spawn.delay = enemy.at("delay").get<float>();
                def.enemies.push_back(spawn);
            }
            lw.waves.push_back(def);
        }
        db.waves[levelId] = lw;
    }

    // Levels
    for (int i = 1; i <= 12; ++i) {
        const std::string levelId = (i < 10 ? "level_0" : "level_") + std::to_string(i);
        auto path = dataPath + "/levels/" + levelId + ".json";
        auto levelJson = loadJsonFile(path);
        data::LevelDefinition def;
        def.id = levelJson.at("id").get<std::string>();
        def.name = levelJson.at("name").get<std::string>();
        def.biome = levelJson.at("biome").get<std::string>();
        def.width = static_cast<int>(levelJson.at("grid").at("width").get<float>());
        def.height = static_cast<int>(levelJson.at("grid").at("height").get<float>());
        def.tileSize = static_cast<int>(levelJson.at("grid").at("tileSize").get<float>());
        def.paths = parsePaths(levelJson.at("paths"));
        def.buildable = parseVector2iArray(levelJson.at("buildable"));
        def.obstacles = parseVector2iArray(levelJson.at("obstacles"));
        for (const auto& rule : levelJson.at("rules")) {
            def.rules.push_back(rule.get<std::string>());
        }
        def.wavesId = levelJson.at("waves").get<std::string>();
        def.startCoins = static_cast<int>(levelJson.at("startCoins").get<float>());
        def.startLives = static_cast<int>(levelJson.at("startLives").get<float>());
        db.levels[def.id] = def;
    }

    // Balance
    auto balanceJson = loadJsonFile(dataPath + "/balance.json");
    db.balance.baseLives = static_cast<int>(balanceJson.at("global").at("baseLives").get<float>());
    db.balance.baseCoins = static_cast<int>(balanceJson.at("global").at("baseCoins").get<float>());
    db.balance.enemyHpMultiplier = balanceJson.at("global").at("enemyHpMultiplier").get<float>();
    db.balance.enemySpeedMultiplier = balanceJson.at("global").at("enemySpeedMultiplier").get<float>();
    db.balance.enemyRewardMultiplier = balanceJson.at("global").at("enemyRewardMultiplier").get<float>();

    for (const auto& entry : balanceJson.at("difficultyCurve")) {
        int level = static_cast<int>(entry.at("level").get<float>());
        float hp = entry.at("hp").get<float>();
        float speed = entry.at("speed").get<float>();
        float reward = entry.at("reward").get<float>();
        db.balance.difficultyCurve[level] = sf::Vector3f{hp, speed, reward};
    }

    for (const auto& [statusId, statusJson] : balanceJson.at("status").get<nlohmann::json::object_t>()) {
        data::StatusDefinition def;
        if (statusJson.contains("multiplier")) def.multiplier = statusJson.at("multiplier").get<float>();
        if (statusJson.contains("duration")) def.duration = statusJson.at("duration").get<float>();
        if (statusJson.contains("dps")) def.dps = statusJson.at("dps").get<float>();
        if (statusJson.contains("stackDps")) def.stackDps = statusJson.at("stackDps").get<float>();
        if (statusJson.contains("stun")) def.stun = statusJson.at("stun").get<float>();
        if (statusJson.contains("armor")) def.armor = statusJson.at("armor").get<float>();
        if (statusJson.contains("magicResist")) def.magicResist = statusJson.at("magicResist").get<float>();
        db.balance.statuses[statusId] = def;
    }

    db.balance.killRewardBonus = balanceJson.at("economy").at("killRewardBonus").get<float>();
    db.balance.waveClearBonus = balanceJson.at("economy").at("waveClearBonus").get<float>();
    db.balance.sellRefund = balanceJson.at("economy").at("sellRefund").get<float>();

    // Settings
    auto settingsJson = loadJsonFile(dataPath + "/settings.json");
    db.settings.audioVolume = settingsJson.value("audioVolume", 1.f);
    db.settings.musicVolume = settingsJson.value("musicVolume", 1.f);
    db.settings.gameSpeed = static_cast<int>(settingsJson.value("gameSpeed", 1));
    db.settings.graphicsQuality = settingsJson.value("graphicsQuality", std::string("medium"));
    db.settings.colorBlindMode = settingsJson.value("colorBlindMode", std::string("normal"));

    // Save
    auto saveJson = loadJsonFile(dataPath + "/save.json");
    db.save.lastUnlockedLevel = static_cast<int>(saveJson.value("lastUnlockedLevel", 1));
    db.save.coins = static_cast<int>(saveJson.value("coins", 0));
    if (saveJson.contains("badges")) {
        for (const auto& badge : saveJson.at("badges")) {
            db.save.badges.push_back(badge.get<std::string>());
        }
    }
    if (saveJson.contains("unlockedTowers")) {
        for (const auto& towerId : saveJson.at("unlockedTowers")) {
            db.save.unlockedTowers.push_back(towerId.get<std::string>());
        }
    }
    if (saveJson.contains("completedLevels")) {
        for (const auto& lvl : saveJson.at("completedLevels")) {
            db.save.completedLevels.push_back(lvl.get<std::string>());
        }
    }

    return db;
}

void DataLoader::saveSettings(const std::string& path, const data::SettingsData& settings) {
    nlohmann::json j;
    j["audioVolume"] = settings.audioVolume;
    j["musicVolume"] = settings.musicVolume;
    j["gameSpeed"] = settings.gameSpeed;
    j["graphicsQuality"] = settings.graphicsQuality;
    j["colorBlindMode"] = settings.colorBlindMode;

    std::ofstream file(path);
    file << j.dump(2);
}

void DataLoader::saveProgress(const std::string& path, const data::SaveData& save) {
    nlohmann::json j;
    j["lastUnlockedLevel"] = save.lastUnlockedLevel;
    j["coins"] = save.coins;
    j["badges"] = save.badges;
    j["unlockedTowers"] = save.unlockedTowers;
    j["completedLevels"] = save.completedLevels;

    std::ofstream file(path);
    file << j.dump(2);
}

} // namespace core

