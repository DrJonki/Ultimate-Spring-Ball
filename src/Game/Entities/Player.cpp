#include <Game/Entities/Player.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

namespace usb
{
  Player::Player(const std::string & name, cpSpace * space)
    : PhysicsBody<sf::CircleShape>(name, sf::Vector2f(100.f, 50.f), 0.f)
  {
    setFillColor(sf::Color::White);
    setRadius(1.f);
    setOrigin(getRadius(), getRadius());

    m_body = cpBodyNew(1.f, cpMomentForCircle(1.f, 0, getRadius(), cpvzero));
    m_shape = cpCircleShapeNew(m_body, getRadius(), cpvzero);
    cpShapeSetElasticity(m_shape, 1.f);

    cpSpaceAddBody(space, m_body);
    cpSpaceAddShape(space, m_shape);

    cpBodySetPosition(m_body, cpv(100.f, 50.f));
  }

  void Player::onMouseClick(const sf::Vector2f & world)
  {
    const auto directionSf = world - getPosition();
    const auto direction = glm::normalize(glm::vec2(
      directionSf.x, directionSf.y
    )) * 750.f;

    cpBodyApplyForceAtLocalPoint(m_body, cpv(direction.x, direction.y), cpvzero);
  }
}
