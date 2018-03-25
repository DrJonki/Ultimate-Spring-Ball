#include <Game/Scenes/Base.hpp>
#include <WTF/System/Network.hpp>

namespace usb
{
  BaseScene::BaseScene()
    : wtf::Scene()
  {
    wtf::Network::setOnMessage([this](sf::Packet& packet) {
      std::string event;
      packet >> event;

      onSocketMessage(event, packet);
    });
  }

  BaseScene::~BaseScene()
  {
    wtf::Network::setOnMessage([](sf::Packet&) {});
  }

  bool BaseScene::connect()
  {
    return wtf::Network::connectTcp("127.0.0.1", 12000);
  }

  void BaseScene::onSocketMessage(const std::string&, sf::Packet&)
  {}
}
