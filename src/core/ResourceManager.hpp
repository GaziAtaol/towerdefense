#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace core {

class ResourceManager {
public:
    ResourceManager() = default;

    bool loadTexture(const std::string& id, const std::string& path) {
        auto texture = std::make_unique<sf::Texture>();
        if (!path.empty() && texture->loadFromFile(path)) {
            texture->setSmooth(true);
            m_textures[id] = std::move(texture);
            return true;
        }
        m_textures[id] = createFallbackTexture();
        return false;
    }

    bool loadSound(const std::string& id, const std::string& path) {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (!path.empty() && buffer->loadFromFile(path)) {
            m_sounds[id] = std::move(buffer);
            return true;
        }
        m_sounds[id] = createSilentBuffer();
        return false;
    }

    bool loadFont(const std::string& id, const std::string& path) {
        auto font = std::make_unique<sf::Font>();
        if (!path.empty() && font->loadFromFile(path)) {
            m_fonts[id] = FontHandle(std::move(font));
            return true;
        }
        m_fonts[id] = FontHandle(&sf::Font::getDefaultFont());
        return false;
    }

    sf::Texture& texture(const std::string& id) { return *m_textures.at(id); }
    const sf::Texture& texture(const std::string& id) const { return *m_textures.at(id); }

    sf::SoundBuffer& sound(const std::string& id) { return *m_sounds.at(id); }
    const sf::SoundBuffer& sound(const std::string& id) const { return *m_sounds.at(id); }

    sf::Font& font(const std::string& id) { return m_fonts.at(id).get(); }
    const sf::Font& font(const std::string& id) const { return m_fonts.at(id).get(); }

private:
    struct FontHandle {
        FontHandle() = default;
        explicit FontHandle(std::unique_ptr<sf::Font> owned)
            : ownedFont(std::move(owned)) {}
        explicit FontHandle(const sf::Font* fallback)
            : fallbackFont(fallback) {}

        sf::Font& get() const {
            if (ownedFont) {
                return *ownedFont;
            }
            return const_cast<sf::Font&>(*fallbackFont);
        }

        std::unique_ptr<sf::Font> ownedFont;
        const sf::Font* fallbackFont{nullptr};
    };

    static std::unique_ptr<sf::Texture> createFallbackTexture() {
        auto texture = std::make_unique<sf::Texture>();
        sf::Image image;
        constexpr unsigned size = 32;
        image.create(size, size, sf::Color::Transparent);
        const sf::Color dark(80, 80, 80);
        const sf::Color light(160, 160, 160);
        for (unsigned y = 0; y < size; ++y) {
            for (unsigned x = 0; x < size; ++x) {
                bool checker = ((x / 8) + (y / 8)) % 2 == 0;
                image.setPixel(x, y, checker ? light : dark);
            }
        }
        texture->loadFromImage(image);
        texture->setSmooth(false);
        return texture;
    }

    static std::unique_ptr<sf::SoundBuffer> createSilentBuffer() {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        std::array<std::int16_t, 2> samples{0, 0};
        buffer->loadFromSamples(samples.data(), samples.size(), 1, 22050);
        return buffer;
    }

    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_sounds;
    std::map<std::string, FontHandle> m_fonts;
};

} // namespace core

