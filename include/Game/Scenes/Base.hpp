#pragma once

#include <WTF/Scene/Scene.hpp>

namespace usb
{
  class BaseScene : public wtf::Scene
  {
  public:

    BaseScene();

  protected:

    bool connect();

  private:

    virtual void onSocketMessage();
  };
}
