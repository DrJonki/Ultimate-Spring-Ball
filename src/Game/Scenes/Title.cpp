#include <Game/Scenes/Title.hpp>
#include <WTF/Scene/Layer.hpp>
#include <WTF/System/Engine.hpp>
#include <WTF/System/Resource.hpp>
#include <cmath>
#include <iostream>

namespace usb
{
  TitleScene::TitleScene()
    : BaseScene()
    , m_connected(std::async(std::launch::async, [this] {
        while (!connect() && wtf::Engine::running());

        return true;
      }))
    , m_mainLayer(addLayer(0))
    , m_connectionText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_singlePlayerText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_multiPlayerText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_multiPlayerConnectionText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_timer(0.f)
  {
    // Text elements
    {
      const auto& font = wtf::Resource::font("helsinki.ttf");

      // Connection text
      m_connectionText.setFont(font);
      m_connectionText.setString("Connecting...");
      m_connectionText.setPosition(10, 10);

      // Single player text
      m_singlePlayerText.setFont(font);
      m_singlePlayerText.setCharacterSize(60);
      m_singlePlayerText.setString("S - Singleplayer");
      m_singlePlayerText.setPosition(10, 800);

      // Multi player text
      m_multiPlayerText.setFont(font);
      m_multiPlayerText.setCharacterSize(60);
      m_multiPlayerText.setFillColor(sf::Color::Red);
      m_multiPlayerText.setString("M - Multiplayer");
      m_multiPlayerText.setPosition(10, 870);

      // Multi player connection description text
      m_multiPlayerConnectionText.setFont(font);
      m_multiPlayerConnectionText.setCharacterSize(22);
      m_multiPlayerConnectionText.setString("Waiting for connection...");
      m_multiPlayerConnectionText.setFillColor(sf::Color::Red);
      m_multiPlayerConnectionText.setPosition(10, 950);
    }
  }

  void TitleScene::update(const float dt)
  {
    if (isConnected()) {
      {
        m_connectionText.setString("Connected!");
        m_connectionText.setFillColor(sf::Color::Green);

        m_multiPlayerText.setFillColor(sf::Color::White);
        m_multiPlayerConnectionText.setActive(false);
      }
    }
    else {
      {
        const auto alpha = static_cast<unsigned char>((std::cos(m_timer += dt * 4.f) + 1.f) / 2 * (UCHAR_MAX / 2) + UCHAR_MAX / 2);

        m_connectionText.setFillColor(sf::Color(255, 255, 255, alpha));
        m_multiPlayerConnectionText.setFillColor(sf::Color(255, 0, 0, alpha));
      }
    }

    
  }

  void TitleScene::onWindowEvent(const sf::Event & event)
  {
    switch (event.type)
    {
      default:
        break;
    }
  }

  bool TitleScene::isConnected() const
  {
    return m_connected.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
  }
}
