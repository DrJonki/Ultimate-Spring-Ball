#pragma once

#include <Game/Entities/PhysicsBody.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace usb
{
  class Player : public PhysicsBody<sf::CircleShape>
  {
  public:

    Player(const std::string& name, cpSpace* space);

    void onMouseClick(const sf::Vector2f& world);
  };
}
