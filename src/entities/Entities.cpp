#include "Entities.hpp"

#include <algorithm>

#include "../math/MathUtils.hpp"

namespace entities {

ecs::Entity spawnEnemy(ecs::Registry& registry, const data::EnemyDefinition& def, const math::Path& path) {
    ecs::Entity entity = registry.create();
    auto& transform = registry.m_transforms[entity];
    if (!path.points.empty()) {
        transform.position = path.points.front();
    }
    auto& renderable = registry.m_renderables[entity];
    renderable.sprite.setColor(sf::Color::Red);
    renderable.sprite.setPosition(transform.position);
    auto& health = registry.m_health[entity];
    health.maxHp = def.hp;
    health.hp = def.hp;
    registry.m_armor[entity].armor = def.armor;
    registry.m_magicResist[entity].resist = def.magicResist;
    auto& stats = registry.m_enemyStats[entity];
    stats.speed = def.speed;
    stats.reward = def.reward;
    stats.pathIndex = 0;
    stats.waypoint = 0;
    stats.progress = 0.f;
    stats.abilities = def.abilities;
    stats.flying = std::find(def.tags.begin(), def.tags.end(), "air") != def.tags.end();
    stats.stealth = std::find(def.abilities.begin(), def.abilities.end(), "stealth") != def.abilities.end();
    stats.stealthTimer = stats.stealth ? 2.5f : 0.f;
    registry.m_statusContainers[entity];
    return entity;
}

ecs::Entity spawnTower(ecs::Registry& registry, const data::TowerDefinition& def, const sf::Vector2f& position) {
    ecs::Entity entity = registry.create();
    registry.m_transforms[entity].position = position;
    registry.m_renderables[entity].sprite.setColor(sf::Color::Blue);
    auto& tower = registry.m_towerStats[entity];
    tower.id = def.id;
    tower.statusEffect = def.statusEffect;
    tower.damage = def.damage;
    tower.fireRate = def.fireRate;
    tower.range = def.range;
    tower.aoeRadius = def.aoeRadius;
    tower.armorPen = def.armorPen;
    tower.chain = def.chain;
    tower.pierce = def.pierce;
    tower.statusPotency = def.statusPotency;
    tower.statusDuration = def.statusDuration;
    tower.canHitFlying = def.canHitFlying;
    if (def.income > 0.f) {
        registry.m_economy[entity].income = def.income;
    }
    registry.m_targeting[entity];
    return entity;
}

} // namespace entities

