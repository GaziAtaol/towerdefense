#include "LevelLoader.hpp"

namespace levels {

LevelRuntime buildLevel(const data::LevelDefinition& def) {
    LevelRuntime runtime;
    runtime.definition = def;
    runtime.paths.reserve(def.paths.size());
    for (const auto& flat : def.paths) {
        math::Path path;
        for (std::size_t i = 0; i + 1 < flat.size(); i += 2) {
            float x = static_cast<float>(flat[i]) * def.tileSize + def.tileSize * 0.5f;
            float y = static_cast<float>(flat[i + 1]) * def.tileSize + def.tileSize * 0.5f;
            path.points.push_back({x, y});
        }
        runtime.paths.push_back(path);
    }
    return runtime;
}

} // namespace levels

