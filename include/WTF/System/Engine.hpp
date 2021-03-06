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

    Engine(const std::string& name);

    template<typename T, typename ... Args>
    static void pushScene(Args&&... args);

    static void exit();
    static bool running();

    static void setWindowClearColor(const sf::Color& color);

    static sf::RenderWindow& getWindow();

    int operator ()();

  private:

    static Engine* ms_instance;

  private:

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    sf::Color m_clearColor;
    std::unique_ptr<Scene> m_scene;
    float m_fixedAccum;
  };

  #include <WTF/System/Engine.inl>
}