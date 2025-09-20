#pragma once

#include <cstdint>

namespace ecs {

using Entity = std::uint32_t;
constexpr Entity InvalidEntity = 0;

inline Entity makeEntity(std::uint32_t index) { return index; }

} // namespace ecs

