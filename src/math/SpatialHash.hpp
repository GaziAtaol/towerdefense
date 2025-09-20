#pragma once

#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <vector>

namespace math {

struct SpatialGridCell {
    std::vector<std::size_t> indices;
};

class SpatialHashGrid {
public:
    SpatialHashGrid(float cellSize = 64.f) : m_cellSize(cellSize) {}

    void clear() { m_cells.clear(); }

    void insert(std::size_t id, const sf::Vector2f& position) {
        auto key = cellKey(position);
        m_cells[key].indices.push_back(id);
    }

    template <typename Fn>
    void query(const sf::Vector2f& position, Fn&& fn) const {
        auto key = cellKey(position);
        auto it = m_cells.find(key);
        if (it != m_cells.end()) {
            for (auto index : it->second.indices) {
                fn(index);
            }
        }
    }

private:
    std::int64_t cellKey(const sf::Vector2f& position) const {
        int x = static_cast<int>(std::floor(position.x / m_cellSize));
        int y = static_cast<int>(std::floor(position.y / m_cellSize));
        return (static_cast<std::int64_t>(x) << 32) ^ y;
    }

    float m_cellSize;
    std::unordered_map<std::int64_t, SpatialGridCell> m_cells;
};

} // namespace math

