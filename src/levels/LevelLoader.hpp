#pragma once

#include "../core/GameData.hpp"
#include "../math/Path.hpp"
#include <vector>

namespace levels {

struct LevelRuntime {
    data::LevelDefinition definition;
    std::vector<math::Path> paths;
};

LevelRuntime buildLevel(const data::LevelDefinition& def);

} // namespace levels

