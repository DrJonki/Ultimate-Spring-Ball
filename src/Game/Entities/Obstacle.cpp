#include <Game/Entities/Obstacle.hpp>
#include <Server/Level/Level.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <chipmunk/chipmunk.h>
#include <rapidjson/pointer.h>
#include <glm/trigonometric.hpp>

namespace usb
{
  Obstacle::Obstacle(const std::string& name, const rapidjson::Value& data, cpSpace* space)
    : wtf::Entity(name)
    , m_drawable()
  {
    namespace rj = rapidjson;
    using usbs::Level;

    const auto type = static_cast<Level::ObstacleType>(rj::Pointer("/type").Get(data)->GetInt());
    const auto angle = rj::Pointer("/angle").Get(data)->GetFloat();
    const sf::Vector2f pos(
      rj::Pointer("/position/0").Get(data)->GetFloat(),
      rj::Pointer("/position/1").Get(data)->GetFloat()
    );
    const sf::Color color(
      static_cast<sf::Uint8>(rj::Pointer("/color/0").Get(data)->GetInt()),
      static_cast<sf::Uint8>(rj::Pointer("/color/1").Get(data)->GetInt()),
      static_cast<sf::Uint8>(rj::Pointer("/color/2").Get(data)->GetInt()),
      static_cast<sf::Uint8>(rj::Pointer("/color/3").Get(data)->GetInt())
    );

    switch (type)
    {
      case Level::ObstacleType::ObstacleRect: {
        const sf::Vector2f size(
          rj::Pointer("/shape/size/0").Get(data)->GetFloat(),
          rj::Pointer("/shape/size/1").Get(data)->GetFloat()
        );
        const auto radius = rj::Pointer("/shape/radius").Get(data)->GetFloat();

        auto drawable = std::make_unique<sf::RectangleShape>(size);
        drawable->setOrigin(size * 0.5f);
        drawable->setPosition(pos);
        drawable->setRotation(angle);
        drawable->setFillColor(color);
        m_drawable = std::move(drawable);

        m_shape = cpSpaceAddShape(space, cpBoxShapeNew(cpSpaceGetStaticBody(space), size.x, size.y, radius));

        break;
      }
      case Level::ObstacleType::ObstacleCircle: {
        const auto radius = rj::Pointer("/shape/radius").Get(data)->GetFloat();

        auto drawable = std::make_unique<sf::CircleShape>(radius);
        drawable->setOrigin(radius, radius);
        drawable->setPosition(pos);
        drawable->setRotation(angle);
        drawable->setFillColor(color);
        m_drawable = std::move(drawable);

        m_shape = cpSpaceAddShape(space, cpCircleShapeNew(cpSpaceGetStaticBody(space), radius, cpvzero));

        break;
      }
    }

    cpShapeSetElasticity(m_shape, rj::Pointer("/elasticity").Get(data)->GetFloat());
    cpBodySetPosition(cpShapeGetBody(m_shape), cpv(pos.x, pos.y));
    cpBodySetAngle(cpShapeGetBody(m_shape), glm::radians(angle));
  }

  void Obstacle::draw(sf::RenderTarget & target) const
  {
    target.draw(*m_drawable);
  }
}
