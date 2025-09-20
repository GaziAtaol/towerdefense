#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"

namespace ecs {

struct Transform {
    sf::Vector2f position{0.f, 0.f};
};

struct Velocity {
    sf::Vector2f velocity{0.f, 0.f};
};

struct Renderable {
    sf::Sprite sprite;
    bool visible = true;
};

struct Health {
    float hp = 0.f;
    float maxHp = 0.f;
};

struct Armor {
    float armor = 0.f;
};

struct MagicResist {
    float resist = 0.f;
};

struct EnemyStats {
    float speed = 0.f;
    int pathIndex = 0;
    int waypoint = 0;
    float progress = 0.f;
    int reward = 0;
    std::vector<std::string> abilities;
    bool flying = false;
    bool stealth = false;
    float stealthTimer = 0.f;
    float speedModifier = 1.f;
    float dotTimer = 0.f;
};

struct TowerStats {
    std::string id;
    std::string statusEffect;
    float damage = 0.f;
    float fireRate = 1.f;
    float cooldown = 0.f;
    float range = 100.f;
    float aoeRadius = 0.f;
    float armorPen = 0.f;
    float chain = 0.f;
    float pierce = 0.f;
    float statusPotency = 0.f;
    float statusDuration = 0.f;
    bool canHitFlying = false;
    int level = 1;
    bool branchASelected = false;
    bool branchBSelected = false;
};

struct Projectile {
    float speed = 0.f;
    float damage = 0.f;
    float armorPen = 0.f;
    float pierce = 0.f;
    float range = 150.f;
    float travelled = 0.f;
    ecs::Entity target = ecs::InvalidEntity;
    std::string statusEffect;
    float statusPower = 0.f;
    float statusDuration = 0.f;
    float aoeRadius = 0.f;
};

struct StatusEffectData {
    std::string id;
    float power = 0.f;
    float duration = 0.f;
    float timeLeft = 0.f;
    int stacks = 0;
};

struct StatusContainer {
    std::vector<StatusEffectData> active;
};

struct Targeting {
    ecs::Entity currentTarget = ecs::InvalidEntity;
    std::string mode = "first";
};

struct Lifetime {
    float ttl = 0.f;
};

struct Owner {
    ecs::Entity owner = ecs::InvalidEntity;
};

struct Experience {
    float xp = 0.f;
};

struct Economy {
    float income = 0.f;
};

struct BuffAura {
    float attackSpeed = 0.f;
    float damage = 0.f;
    float range = 0.f;
    float armorPen = 0.f;
    float statusResist = 0.f;
    float sellRefund = 0.f;
};

struct ProjectilePoolTag {};
struct EffectPoolTag {};

} // namespace ecs

