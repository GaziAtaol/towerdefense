#pragma once

#include "../core/GameData.hpp"
#include "../ecs/Registry.hpp"
#include "../math/Path.hpp"
#include <SFML/Graphics.hpp>

namespace entities {

ecs::Entity spawnEnemy(ecs::Registry& registry, const data::EnemyDefinition& def, const math::Path& path);
ecs::Entity spawnTower(ecs::Registry& registry, const data::TowerDefinition& def, const sf::Vector2f& position);

}

