#include "Systems.hpp"

#include <algorithm>

namespace systems {

static std::string towerTargetingMode(ecs::Entity e, ecs::Registry& registry);

void updateMovement(ecs::Registry& registry, const PathContext& pathContext, float dt, float tileSize, int& livesLost) {
    (void)tileSize;
    std::vector<ecs::Entity> toRemove;
    if (pathContext.paths.empty()) {
        return;
    }
    for (auto& [entity, stats] : registry.m_enemyStats) {
        auto& transform = registry.m_transforms[entity];
        stats.speedModifier = std::max(0.1f, stats.speedModifier);
        float moveDistance = stats.speed * stats.speedModifier * dt;
        auto& path = pathContext.paths[stats.pathIndex];
        bool reachedEnd = false;
        while (moveDistance > 0.f && !reachedEnd) {
            if (stats.waypoint >= static_cast<int>(path.points.size()) - 1) {
                reachedEnd = true;
                break;
            }
            sf::Vector2f a = path.points[stats.waypoint];
            sf::Vector2f b = path.points[stats.waypoint + 1];
            float segmentLength = math::distance(a, b);
            float remaining = (1.f - stats.progress) * segmentLength;
            if (moveDistance < remaining) {
                stats.progress += moveDistance / segmentLength;
                moveDistance = 0.f;
            } else {
                moveDistance -= remaining;
                stats.waypoint++;
                stats.progress = 0.f;
                if (stats.waypoint >= static_cast<int>(path.points.size()) - 1) {
                    reachedEnd = true;
                }
            }
        }

        if (reachedEnd) {
            toRemove.push_back(entity);
            ++livesLost;
            continue;
        }

        if (stats.waypoint < static_cast<int>(path.points.size()) - 1) {
            sf::Vector2f a = path.points[stats.waypoint];
            sf::Vector2f b = path.points[stats.waypoint + 1];
            transform.position = math::lerp(a, b, stats.progress);
        }
    }

    for (auto entity : toRemove) {
        registry.destroy(entity);
    }
}

static float targetingScore(const std::string& mode, const ecs::EnemyStats& enemy, float distance, float hp, float armor) {
    if (mode == "closest") return -distance;
    if (mode == "highest_hp") return hp;
    if (mode == "lowest_armor") return -armor;
    if (mode == "last") return enemy.waypoint + enemy.progress;
    return -(enemy.waypoint + enemy.progress);
}

void updateTargeting(ecs::Registry& registry, const math::SpatialHashGrid& grid, const std::vector<ecs::Entity>& enemyOrder, float) {
    for (auto& [entity, tower] : registry.m_towerStats) {
        auto& transform = registry.m_transforms[entity];
        ecs::Entity bestTarget = ecs::InvalidEntity;
        float bestScore = -1e9f;
        grid.query(transform.position, [&](std::size_t idx) {
            if (idx >= enemyOrder.size()) return;
            ecs::Entity candidate = enemyOrder[idx];
            auto enemyTransformIt = registry.m_transforms.find(candidate);
            auto enemyStatsIt = registry.m_enemyStats.find(candidate);
            auto healthIt = registry.m_health.find(candidate);
            if (enemyTransformIt == registry.m_transforms.end() || enemyStatsIt == registry.m_enemyStats.end() || healthIt == registry.m_health.end()) return;
            const auto& enemyStats = enemyStatsIt->second;
            if (enemyStats.flying && !tower.canHitFlying) return;
            if (enemyStats.stealth && enemyStats.stealthTimer > 0.f) return;
            const auto& enemyPos = enemyTransformIt->second.position;
            float dist = math::distance(transform.position, enemyPos);
            if (dist > tower.range) return;
            float armor = 0.f;
            auto armorIt = registry.m_armor.find(candidate);
            if (armorIt != registry.m_armor.end()) armor = armorIt->second.armor;
            float score = targetingScore(towerTargetingMode(entity, registry), enemyStats, dist, healthIt->second.hp, armor);
            if (score > bestScore) {
                bestScore = score;
                bestTarget = candidate;
            }
        });
        registry.m_targeting[entity].currentTarget = bestTarget;
    }
}

void updateFiring(ecs::Registry& registry, ProjectilePool& pool, float dt, const data::BalanceDefinition&) {
    for (auto& [entity, tower] : registry.m_towerStats) {
        tower.cooldown -= dt;
        if (tower.cooldown > 0.f) continue;
        auto targetIt = registry.m_targeting.find(entity);
        if (targetIt == registry.m_targeting.end() || targetIt->second.currentTarget == ecs::InvalidEntity) continue;
        ecs::Entity target = targetIt->second.currentTarget;
        if (!registry.m_transforms.count(target) || !registry.m_health.count(target)) continue;

        ecs::Entity projectileEntity;
        if (!pool.available.empty()) {
            projectileEntity = pool.available.back();
            pool.available.pop_back();
        } else {
            projectileEntity = registry.create();
        }

        auto& transform = registry.m_transforms[projectileEntity];
        transform.position = registry.m_transforms[entity].position;
        auto& projectile = registry.m_projectiles[projectileEntity];
        projectile.speed = 420.f;
        projectile.damage = tower.damage;
        projectile.armorPen = tower.armorPen;
        projectile.pierce = tower.pierce;
        projectile.range = tower.range + 40.f;
        projectile.travelled = 0.f;
        projectile.target = target;
        projectile.statusEffect = (tower.statusPotency > 0.f && !tower.statusEffect.empty()) ? tower.statusEffect : std::string{};
        projectile.statusPower = tower.statusPotency;
        projectile.statusDuration = tower.statusDuration;
        projectile.aoeRadius = tower.aoeRadius;

        tower.cooldown = std::max(0.1f, 1.f / std::max(0.1f, tower.fireRate));
    }
}

void updateProjectiles(ecs::Registry& registry, float dt, const data::BalanceDefinition&) {
    std::vector<ecs::Entity> toDestroy;
    for (auto& [entity, projectile] : registry.m_projectiles) {
        auto& transform = registry.m_transforms[entity];
        if (!registry.m_transforms.count(projectile.target)) {
            toDestroy.push_back(entity);
            continue;
        }
        sf::Vector2f targetPos = registry.m_transforms[projectile.target].position;
        sf::Vector2f dir = targetPos - transform.position;
        float distance = math::length(dir);
        if (distance <= 5.f) {
            if (registry.m_health.count(projectile.target)) {
                auto& health = registry.m_health[projectile.target];
                float armor = 0.f;
                auto armorIt = registry.m_armor.find(projectile.target);
                if (armorIt != registry.m_armor.end()) armor = armorIt->second.armor;
                float mitigation = std::max(0.f, armor - projectile.armorPen);
                float effective = projectile.damage * (1.f - mitigation / 100.f);
                health.hp -= effective;
                if (!projectile.statusEffect.empty() && registry.m_statusContainers.count(projectile.target)) {
                    auto& statuses = registry.m_statusContainers[projectile.target].active;
                    bool found = false;
                    for (auto& status : statuses) {
                        if (status.id == projectile.statusEffect) {
                            status.power += projectile.statusPower;
                            status.timeLeft = std::max(status.timeLeft, projectile.statusDuration);
                            status.stacks++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        statuses.push_back({projectile.statusEffect, projectile.statusPower, projectile.statusDuration, projectile.statusDuration, 1});
                    }
                }
            }
            toDestroy.push_back(entity);
            continue;
        }
        sf::Vector2f step = math::normalize(dir) * projectile.speed * dt;
        transform.position += step;
        projectile.travelled += math::length(step);
        if (projectile.travelled > projectile.range) {
            toDestroy.push_back(entity);
        }
    }

    for (auto entity : toDestroy) {
        registry.m_projectiles.erase(entity);
        registry.destroy(entity);
    }
}

void updateStatus(ecs::Registry& registry, float dt, const data::BalanceDefinition& balance) {
    for (auto& [entity, container] : registry.m_statusContainers) {
        if (!registry.m_enemyStats.count(entity)) continue;
        auto& enemy = registry.m_enemyStats[entity];
        enemy.speedModifier = 1.f;
        for (auto& status : container.active) {
            status.timeLeft -= dt;
            auto it = balance.statuses.find(status.id);
            if (it == balance.statuses.end()) continue;
            const auto& def = it->second;
            if (def.multiplier > 0.f && def.multiplier < 1.f) {
                enemy.speedModifier *= def.multiplier;
            }
            if (status.id.find("slow") != std::string::npos) {
                enemy.speedModifier *= std::max(0.2f, 1.f - status.power);
            }
            if (def.dps > 0.f) {
                auto healthIt = registry.m_health.find(entity);
                if (healthIt != registry.m_health.end()) {
                    healthIt->second.hp -= (def.dps + def.stackDps * std::max(0, status.stacks - 1)) * dt;
                }
            }
            if (def.stun > 0.f) {
                enemy.speedModifier = 0.f;
            }
        }
        container.active.erase(std::remove_if(container.active.begin(), container.active.end(), [](const ecs::StatusEffectData& s) { return s.timeLeft <= 0.f; }), container.active.end());
    }
}

void updateCleanup(ecs::Registry& registry, ProjectilePool& projectilePool, EffectPool&) {
    std::vector<ecs::Entity> toRemove;
    for (auto& [entity, health] : registry.m_health) {
        if (health.hp <= 0.f) {
            toRemove.push_back(entity);
        }
    }
    for (auto entity : toRemove) {
        if (registry.m_projectiles.count(entity)) {
            projectilePool.available.push_back(entity);
            registry.m_projectiles.erase(entity);
        }
        registry.destroy(entity);
    }
}

static std::string towerTargetingMode(ecs::Entity e, ecs::Registry& registry) {
    auto it = registry.m_targeting.find(e);
    if (it != registry.m_targeting.end()) return it->second.mode;
    return "first";
}

} // namespace systems

