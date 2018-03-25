#include <WTF/System/Engine.hpp>
#include <WTF/System/Config.hpp>
#include <WTF/System/Resource.hpp>
#include <WTF/System/Network.hpp>
#include <WTF/Scene/Scene.hpp>
#include <SFML/Window/Event.hpp>

namespace wtf
{
  Engine* Engine::ms_instance = nullptr;

  Engine::Engine(const std::string& name)
    : m_window()
    , m_clock()
    , m_clearColor(sf::Color::Black)
    , m_scene()
    , m_fixedAccum(0.f)
  {
    if (ms_instance) {
      throw "Only one wtf::Engine instance may exist";
    }

    ms_instance = this;

    m_window.setVerticalSyncEnabled(true);
    m_window.setKeyRepeatEnabled(false);
    m_window.setMouseCursorVisible(false);

    m_window.create(sf::VideoMode(
      Config::get<int>("WINDOW_X"),
      Config::get<int>("WINDOW_Y")
    ), name);
  }

  void Engine::exit()
  {
    ms_instance->m_window.close();

    Resource::clearAll();
  }

  bool Engine::running()
  {
    return ms_instance->m_window.isOpen();
  }

  void Engine::setWindowClearColor(const sf::Color & color)
  {
    ms_instance->m_clearColor = color;
  }

  int Engine::operator ()()
  {
    while (running()) {
      static const auto fixedStep = 1.f / Config::get<int>("FIXED_TICKRATE");
      const auto delta = m_clock.restart().asSeconds();

      Network::pollMessages();
      m_window.clear(m_clearColor);

      if (m_scene) {
        m_scene->update(delta);

        if ((m_fixedAccum += delta) >= fixedStep) {
          m_fixedAccum -= fixedStep;

          m_scene->fixedUpdate(fixedStep);
        }

        m_scene->draw(m_window);
      }

      m_window.display();

      // Handle events
      sf::Event event;
      while (m_window.pollEvent(event)) {
        switch (event.type)
        {
          case sf::Event::Closed:
            exit();
            break;
          /*case sf::Event::Resized:
          {
            const auto view = m_window.getView().getSize();
            const auto ratio = view.x / view.y;

            m_window.setSize(sf::Vector2u(
              static_cast<unsigned int>(m_window.getSize().y * ratio),
              m_window.getSize().y
            ));

            break;
          }*/

          default:
            if (m_scene) {
              m_scene->onWindowEvent(event);
            }
        }
      }
    }
    
    return 0;
  }

  sf::RenderWindow & Engine::getWindow()
  {
    return ms_instance->m_window;
  }
}
