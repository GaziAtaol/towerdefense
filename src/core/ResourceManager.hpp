#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>

namespace core {

class ResourceManager {
public:
    bool loadTexture(const std::string& id, const std::string& path) {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(path)) {
            return false;
        }
        texture->setSmooth(true);
        m_textures[id] = std::move(texture);
        return true;
    }

    bool loadSound(const std::string& id, const std::string& path) {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (!buffer->loadFromFile(path)) {
            return false;
        }
        m_sounds[id] = std::move(buffer);
        return true;
    }

    bool loadFont(const std::string& id, const std::string& path) {
        auto font = std::make_unique<sf::Font>();
        if (!font->loadFromFile(path)) {
            return false;
        }
        m_fonts[id] = std::move(font);
        return true;
    }

    sf::Texture& texture(const std::string& id) { return *m_textures.at(id); }
    const sf::Texture& texture(const std::string& id) const { return *m_textures.at(id); }

    sf::SoundBuffer& sound(const std::string& id) { return *m_sounds.at(id); }
    const sf::SoundBuffer& sound(const std::string& id) const { return *m_sounds.at(id); }

    sf::Font& font(const std::string& id) { return *m_fonts.at(id); }
    const sf::Font& font(const std::string& id) const { return *m_fonts.at(id); }

private:
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_sounds;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};

} // namespace core

