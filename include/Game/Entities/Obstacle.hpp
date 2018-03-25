#pragma once

#include <WTF/Scene/Entity.hpp>
#include <chipmunk/chipmunk_structs.h>
#include <rapidjson/document.h>
#include <memory>

namespace sf
{
  class Drawable;
}

namespace usb
{
  class Obstacle : public wtf::Entity
  {
  public:

    Obstacle(const std::string& name, const rapidjson::Value& data, cpSpace* space);

  private:

    void draw(sf::RenderTarget& target) const override;

  private:

    std::unique_ptr<sf::Drawable> m_drawable;
    cpShape* m_shape;
  };
}
