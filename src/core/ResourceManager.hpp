#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace core {

class ResourceManager {
public:
    void setAssetRoot(std::filesystem::path root);

    bool loadTexture(const std::string& id, const std::filesystem::path& path);
    bool loadSound(const std::string& id, const std::filesystem::path& path);
    bool loadFont(const std::string& id, const std::filesystem::path& path);

    sf::Texture& texture(const std::string& id) { return *m_textures.at(id); }
    const sf::Texture& texture(const std::string& id) const { return *m_textures.at(id); }

    sf::SoundBuffer& sound(const std::string& id) { return *m_sounds.at(id); }
    const sf::SoundBuffer& sound(const std::string& id) const { return *m_sounds.at(id); }

    sf::Font& font(const std::string& id) { return *m_fonts.at(id); }
    const sf::Font& font(const std::string& id) const { return *m_fonts.at(id); }

private:
    std::filesystem::path resolvePath(const std::filesystem::path& path) const;
    bool tryLoadFont(sf::Font& font, const std::filesystem::path& path) const;
    std::vector<std::filesystem::path> fontFallbackCandidates() const;
    void generatePlaceholderTexture(sf::Texture& texture) const;
    void generateSilentSound(sf::SoundBuffer& buffer) const;

    std::filesystem::path m_assetRoot;
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_sounds;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};

} // namespace core

