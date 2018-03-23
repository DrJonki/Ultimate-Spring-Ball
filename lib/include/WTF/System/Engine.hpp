#pragma once

#include <WTF/Macros.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace wtf
{
  class Scene;

  class Engine final
  {
    WTF_NO_COPY(Engine);

  public:

    Engine();

    void operator ()();

  private:

    sf::Clock m_clock;
  };
}