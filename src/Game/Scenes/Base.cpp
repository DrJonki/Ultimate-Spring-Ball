#include <Game/Scenes/Base.hpp>
#include <WTF/System/Network.hpp>

namespace usb
{
  BaseScene::BaseScene()
    : wtf::Scene()
  {}

  bool BaseScene::connect()
  {
    return wtf::Network::connectTcp("127.0.0.1", 12000);
  }

  void BaseScene::onSocketMessage()
  {}
}
