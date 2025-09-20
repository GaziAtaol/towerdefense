#include "ResourceManager.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace core {

namespace {
constexpr unsigned int kGeneratedTextureSize = 64;
constexpr unsigned int kSilentSampleRate = 22050;
constexpr unsigned int kSilentDurationMs = 250;
} // namespace

void ResourceManager::setAssetRoot(std::filesystem::path root) {
    m_assetRoot = std::move(root);
}

std::filesystem::path ResourceManager::resolvePath(const std::filesystem::path& path) const {
    if (path.empty()) {
        return {};
    }
    if (path.is_absolute() || m_assetRoot.empty()) {
        return path;
    }
    return m_assetRoot / path;
}

void ResourceManager::generatePlaceholderTexture(sf::Texture& texture) const {
    sf::Image image;
    image.create(kGeneratedTextureSize, kGeneratedTextureSize, sf::Color(160, 160, 160));
    for (unsigned int y = 0; y < kGeneratedTextureSize; ++y) {
        for (unsigned int x = 0; x < kGeneratedTextureSize; ++x) {
            if ((x / 8 + y / 8) % 2 == 0) {
                image.setPixel(x, y, sf::Color(110, 110, 110));
            }
        }
    }
    if (!texture.loadFromImage(image)) {
        throw std::runtime_error("Failed to create procedural placeholder texture");
    }
    texture.setSmooth(false);
}

void ResourceManager::generateSilentSound(sf::SoundBuffer& buffer) const {
    const unsigned int sampleCount = kSilentSampleRate * kSilentDurationMs / 1000;
    std::vector<sf::Int16> samples(sampleCount, 0);
    if (!buffer.loadFromSamples(samples.data(), samples.size(), 1, kSilentSampleRate)) {
        throw std::runtime_error("Failed to create procedural silent sound");
    }
}

bool ResourceManager::loadTexture(const std::string& id, const std::filesystem::path& path) {
    auto texture = std::make_unique<sf::Texture>();
    const auto resolved = resolvePath(path);
    bool loadedFromFile = false;
    if (!resolved.empty()) {
        std::error_code ec;
        if (std::filesystem::exists(resolved, ec)) {
            loadedFromFile = texture->loadFromFile(resolved.string());
        }
    }
    if (!loadedFromFile) {
        generatePlaceholderTexture(*texture);
        std::cerr << "[ResourceManager] Using generated texture for '" << id << "' (missing file: " << resolved
                  << ")\n";
    } else {
        texture->setSmooth(true);
    }
    m_textures[id] = std::move(texture);
    return loadedFromFile;
}

bool ResourceManager::loadSound(const std::string& id, const std::filesystem::path& path) {
    auto buffer = std::make_unique<sf::SoundBuffer>();
    const auto resolved = resolvePath(path);
    bool loadedFromFile = false;
    if (!resolved.empty()) {
        std::error_code ec;
        if (std::filesystem::exists(resolved, ec)) {
            loadedFromFile = buffer->loadFromFile(resolved.string());
        }
    }
    if (!loadedFromFile) {
        generateSilentSound(*buffer);
        std::cerr << "[ResourceManager] Using generated silent sound for '" << id << "' (missing file: " << resolved
                  << ")\n";
    }
    m_sounds[id] = std::move(buffer);
    return loadedFromFile;
}

bool ResourceManager::tryLoadFont(sf::Font& font, const std::filesystem::path& path) const {
    if (path.empty()) {
        return false;
    }
    std::error_code ec;
    if (!std::filesystem::exists(path, ec)) {
        return false;
    }
    return font.loadFromFile(path.string());
}

std::vector<std::filesystem::path> ResourceManager::fontFallbackCandidates() const {
    std::vector<std::filesystem::path> candidates;
    if (!m_assetRoot.empty()) {
        candidates.push_back(m_assetRoot / "fonts/DejaVuSans.ttf");
    }
    if (const char* env = std::getenv("TOWERDEFENSE_FONT")) {
        candidates.emplace_back(env);
    }
#ifdef _WIN32
    if (const char* windir = std::getenv("WINDIR")) {
        std::filesystem::path base(windir);
        candidates.push_back(base / "Fonts/arial.ttf");
        candidates.push_back(base / "Fonts/segoeui.ttf");
    }
    candidates.emplace_back("C:/Windows/Fonts/arial.ttf");
    candidates.emplace_back("C:/Windows/Fonts/segoeui.ttf");
#elif __APPLE__
    candidates.emplace_back("/System/Library/Fonts/Supplemental/Helvetica.ttf");
    candidates.emplace_back("/System/Library/Fonts/Supplemental/Arial Unicode.ttf");
#else
    candidates.emplace_back("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    candidates.emplace_back("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    candidates.emplace_back("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    candidates.emplace_back("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
#endif
    return candidates;
}

bool ResourceManager::loadFont(const std::string& id, const std::filesystem::path& path) {
    auto font = std::make_unique<sf::Font>();
    const auto resolved = resolvePath(path);
    if (tryLoadFont(*font, resolved)) {
        m_fonts[id] = std::move(font);
        return true;
    }
    for (const auto& candidate : fontFallbackCandidates()) {
        if (tryLoadFont(*font, candidate)) {
            std::cerr << "[ResourceManager] Loaded font '" << id << "' from fallback: " << candidate << "\n";
            m_fonts[id] = std::move(font);
            return true;
        }
    }
    throw std::runtime_error("Failed to load font '" + id + "' from '" + resolved.string() + "' or fallbacks");
}

} // namespace core
