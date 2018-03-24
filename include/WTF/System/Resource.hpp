#pragma once

#include <WTF/Export.hpp>
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
  class WTF_API Resource
  {
    public:

      static sf::Texture& texture(const std::string& path);
      static sf::SoundBuffer& soundBuffer(const std::string& path);
      static sf::Font& font(const std::string& path);

      static void clearAll();
  };
}