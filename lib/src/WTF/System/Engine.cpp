#include <WTF/System/Engine.hpp>

namespace wtf
{
  Engine::Engine()
  {
    /*window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);*/
  }

  void Engine::operator ()()
  {
    const auto delta = m_clock.restart().asSeconds();

    //if (currentScene) {
    //  currentScene->update(delta);
    //  currentScene->draw(m_window);
    //}

    //window.display();

    //// Handle events
    //sf::Event event;
    //while (window.pollEvent(event)) {
    //  switch (event.type)
    //  {
    //    case sf::Event::Closed:
    //      if (currentScene) {
    //        currentScene.reset();
    //      }

    //      window.close();

    //      break;
    //    case sf::Event::Resized:
    //    {
    //      const auto view = window.getView().getSize();
    //      const auto ratio = view.x / view.y;

    //      window.setSize(sf::Vector2u(
    //        static_cast<unsigned int>(window.getSize().y * ratio),
    //        window.getSize().y
    //      ));

    //      break;
    //    }
    //  }
    //}
  }
}
