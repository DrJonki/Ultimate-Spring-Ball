#include <Game/Scenes/Title.hpp>
#include <Game/Scenes/Game.hpp>
#include <WTF/Util/Randomizer.hpp>
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
    , m_timer(0.f)
    , m_music()
    , m_mainLayer(addLayer(0))
    , m_connectionText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_singlePlayerText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_multiPlayerText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_multiPlayerConnectionText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
    , m_titleText(m_mainLayer.insert<wtf::Drawable<sf::Text>>(""))
  {
    // Music
    {
      m_music.openFromFile("assets/Music/And_-_01_-_Gigababoki.ogg");
      m_music.setRelativeToListener(true);
      m_music.setLoop(true);
      m_music.play();
    }

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

      // Title text
      {
        m_titleText.setFont(font);
        m_titleText.setCharacterSize(100);
        m_titleText.setFillColor(sf::Color::Yellow);
        m_titleText.setString("  ULTIMATE\nSPRING BALL");

        const auto bounds = m_titleText.getGlobalBounds();
        m_titleText.setOrigin(bounds.width / 2, bounds.height / 2);
        m_titleText.setPosition(500, 400);
      }
    }
  }

  void TitleScene::update(const float dt)
  {
    m_timer += dt;

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
        const auto alpha = static_cast<unsigned char>((std::cos(m_timer * 5) + 1.f) / 2 * (UCHAR_MAX / 2) + UCHAR_MAX / 2);

        m_connectionText.setFillColor(sf::Color(255, 255, 255, alpha));
        m_multiPlayerConnectionText.setFillColor(sf::Color(255, 0, 0, alpha));
      }
    }

    const auto scale = 0.75f + (std::cos(m_timer * 11) + 1.f) / 2.f / 4.f;
    const auto rotation = std::sin(m_timer * 5) * 22;
    m_titleText.setScale(scale, scale);
    m_titleText.setRotation(rotation);

    BaseScene::update(dt);
  }

  void TitleScene::onWindowEvent(const sf::Event & event)
  {
    switch (event.type)
    {
      case sf::Event::EventType::KeyReleased: {
        if (event.key.code == sf::Keyboard::Key::S) {
          wtf::Engine::pushScene<GameScene>(true);
        }

        break;
      }
    }
  }

  bool TitleScene::isConnected() const
  {
    return m_connected.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
  }
}
