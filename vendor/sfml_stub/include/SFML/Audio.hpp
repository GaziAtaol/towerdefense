#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace sf {

class SoundBuffer {
public:
    bool loadFromFile(const std::string&) { return true; }
    bool loadFromSamples(const std::int16_t*, std::size_t, unsigned int, unsigned int) { return true; }
};

} // namespace sf

