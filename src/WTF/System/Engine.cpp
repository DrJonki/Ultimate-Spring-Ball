#include <WTF/System/Engine.hpp>
#include <WTF/System/Config.hpp>
#include <WTF/Scene/Scene.hpp>
#include <SFML/Window/Event.hpp>

namespace wtf
{
  Engine* Engine::ms_instance = nullptr;

  Engine::Engine()
    : m_window()
    , m_clock()
    , m_scene()
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
    ), "Ultimate Spring Ball");
  }

  void Engine::exit()
  {
    ms_instance->m_window.close();
  }

  int Engine::operator ()()
  {
    while (m_window.isOpen()) {
      const auto delta = m_clock.restart().asSeconds();

      if (m_scene) {
        m_scene->update(delta);
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
          case sf::Event::Resized:
          {
            const auto view = m_window.getView().getSize();
            const auto ratio = view.x / view.y;

            m_window.setSize(sf::Vector2u(
              static_cast<unsigned int>(m_window.getSize().y * ratio),
              m_window.getSize().y
            ));

            break;
          }
        }
      }
    }
    
    return 0;
  }
}
