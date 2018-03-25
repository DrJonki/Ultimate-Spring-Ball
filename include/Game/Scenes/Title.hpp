#pragma once

#include <Game/Scenes/Base.hpp>
#include <WTF/Scene/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>
#include <future>

namespace wtf
{
  class Layer;
}

namespace usb
{
  class TitleScene : public BaseScene
  {
  public:

    TitleScene();

  private:

    void update(const float dt) override;

    void onWindowEvent(const sf::Event& event) override;

    bool isConnected() const;

  private:

    std::future<bool> m_connected;
    float m_timer;

    // Music
    sf::Music m_music;

    // Layers
    wtf::Layer& m_mainLayer;

    // Texts
    wtf::Drawable<sf::Text>& m_connectionText;
    wtf::Drawable<sf::Text>& m_singlePlayerText;
    wtf::Drawable<sf::Text>& m_multiPlayerText;
    wtf::Drawable<sf::Text>& m_multiPlayerConnectionText;
    wtf::Drawable<sf::Text>& m_titleText;
  };
}
