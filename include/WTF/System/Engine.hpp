#pragma once

#include <WTF/Export.hpp>
#include <WTF/Macros.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace wtf
{
  class Scene;

  class WTF_API Engine final
  {
    WTF_NO_COPY(Engine);

  public:

    Engine();

    template<typename T, typename ... Args>
    static void pushScene(Args&&... args);

    static void exit();

    int operator ()();

  private:

    static Engine* ms_instance;

  private:

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    std::unique_ptr<Scene> m_scene;
  };

  #include <WTF/System/Engine.inl>
}