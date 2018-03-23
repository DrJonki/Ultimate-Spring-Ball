#include <WTF/System/Resource.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include <cassert>
#include <unordered_map>

namespace
{
  std::unordered_map<std::string, sf::Texture> ns_textures;
  std::unordered_map<std::string, sf::SoundBuffer> ns_soundbuffers;
  std::unordered_map<std::string, sf::Font> ns_fonts;
}

namespace wtf
{
  sf::Texture& Resource::texture(const std::string& path)
  {
    const std::string assetPath = "assets/Textures/" + path;
    auto itr = ns_textures.find(assetPath);

    if (itr == ns_textures.end()) {
      sf::Texture& texture = ns_textures[assetPath];
      if (!texture.loadFromFile(assetPath)) {
        assert(false);
      }

      return texture;
    }

    return itr->second;
  }

  sf::SoundBuffer& Resource::soundBuffer(const std::string& path)
  {
    std::string assetPath = "assets/Audio/" + path;

    auto itr = ns_soundbuffers.find(assetPath);

    if (itr == ns_soundbuffers.end()) {
      sf::SoundBuffer& soundBuff = ns_soundbuffers[assetPath];
      if (!soundBuff.loadFromFile(assetPath)) {
        assert(false);
      }

      return soundBuff;
    }

    return itr->second;
  }

  sf::Font& Resource::font(const std::string& path)
  {
    std::string assetPath = "assets/Fonts/" + path;
    auto itr = ns_fonts.find(assetPath);

    if (itr == ns_fonts.end()) {
      sf::Font& font = ns_fonts[assetPath];
      if (!font.loadFromFile(assetPath)) {
        assert(false);
      }

      return font;
    }

    return itr->second;
  }
}
