#pragma once

#include "../core/GameData.hpp"
#include "../math/MathUtils.hpp"
#include "../math/Path.hpp"
#include "../math/SpatialHash.hpp"
#include "../ecs/Registry.hpp"
#include <unordered_map>
#include <vector>

namespace systems {

struct PathContext {
    std::vector<math::Path> paths;
};

struct ProjectilePool {
    std::vector<ecs::Entity> available;
};

struct EffectPool {
    std::vector<ecs::Entity> available;
};

void updateMovement(ecs::Registry& registry, const PathContext& pathContext, float dt, float tileSize, int& livesLost);
void updateTargeting(ecs::Registry& registry, const math::SpatialHashGrid& grid, const std::vector<ecs::Entity>& enemyOrder, float dt);
void updateFiring(ecs::Registry& registry, ProjectilePool& pool, float dt, const data::BalanceDefinition& balance);
void updateProjectiles(ecs::Registry& registry, float dt, const data::BalanceDefinition& balance);
void updateStatus(ecs::Registry& registry, float dt, const data::BalanceDefinition& balance);
void updateCleanup(ecs::Registry& registry, ProjectilePool& projectilePool, EffectPool& effectPool);

} // namespace systems

