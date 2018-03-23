#include <Game/Scenes/Connecting.hpp>
#include <WTF/Scene/Layer.hpp>
#include <WTF/Scene/Drawable.hpp>
#include <WTF/System/Engine.hpp>
#include <WTF/System/Resource.hpp>
#include <WTF/System/Network.hpp>
#include <SFML/Graphics/Text.hpp>

namespace usb
{
  ConnectingScene::ConnectingScene()
    : wtf::Scene()
  {
    auto& layer = addLayer(0);

    auto& connectingText = layer.insert<wtf::Drawable<sf::Text>>("");

    connectingText.setFont(wtf::Resource::font("helsinki.ttf"));
    connectingText.setString("Connecting");

    wtf::Network::connectTcp("127.0.0.1", 12000);

    wtf::Engine::exit();
  }
}
