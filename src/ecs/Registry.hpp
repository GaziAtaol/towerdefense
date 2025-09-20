#pragma once

#include "Components.hpp"
#include <unordered_map>

namespace ecs {

class Registry {
public:
    Entity create() {
        Entity e = ++m_last;
        m_transforms[e];
        return e;
    }

    void destroy(Entity e) {
        m_transforms.erase(e);
        m_velocities.erase(e);
        m_renderables.erase(e);
        m_health.erase(e);
        m_armor.erase(e);
        m_magicResist.erase(e);
        m_enemyStats.erase(e);
        m_towerStats.erase(e);
        m_projectiles.erase(e);
        m_statusContainers.erase(e);
        m_targeting.erase(e);
        m_lifetimes.erase(e);
        m_owners.erase(e);
        m_experience.erase(e);
        m_economy.erase(e);
        m_buffAura.erase(e);
    }

    std::unordered_map<Entity, Transform> m_transforms;
    std::unordered_map<Entity, Velocity> m_velocities;
    std::unordered_map<Entity, Renderable> m_renderables;
    std::unordered_map<Entity, Health> m_health;
    std::unordered_map<Entity, Armor> m_armor;
    std::unordered_map<Entity, MagicResist> m_magicResist;
    std::unordered_map<Entity, EnemyStats> m_enemyStats;
    std::unordered_map<Entity, TowerStats> m_towerStats;
    std::unordered_map<Entity, Projectile> m_projectiles;
    std::unordered_map<Entity, StatusContainer> m_statusContainers;
    std::unordered_map<Entity, Targeting> m_targeting;
    std::unordered_map<Entity, Lifetime> m_lifetimes;
    std::unordered_map<Entity, Owner> m_owners;
    std::unordered_map<Entity, Experience> m_experience;
    std::unordered_map<Entity, Economy> m_economy;
    std::unordered_map<Entity, BuffAura> m_buffAura;

private:
    Entity m_last = 0;
};

} // namespace ecs

