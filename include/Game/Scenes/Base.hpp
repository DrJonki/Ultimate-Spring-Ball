#pragma once

#include <WTF/Scene/Scene.hpp>

namespace sf
{
  class Packet;
}

namespace usb
{
  class BaseScene : public wtf::Scene
  {
  public:

    BaseScene();

    virtual ~BaseScene() override;

  protected:

    bool connect();

  private:

    virtual void onSocketMessage(const std::string& event, sf::Packet& packet);
  };
}
