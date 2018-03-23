#pragma once

#include <string>

namespace sf
{
  class Texture;
  class SoundBuffer;
  class Font;
  class Shader;
}

namespace wtf
{
  class Resource
  {
    public:

      sf::Texture& texture(const std::string& path);
      sf::SoundBuffer& soundBuffer(const std::string& path);
      sf::Font& font(const std::string& path);
  };
}