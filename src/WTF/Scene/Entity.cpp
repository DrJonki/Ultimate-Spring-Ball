#include <WTF/Scene/Entity.hpp>

namespace wtf
{
  Entity::Entity(const std::string& name)
    : m_name    (name)
    , m_active  (true)
    , m_deleted (false)
  {}

  Entity::~Entity()
  {}

  void Entity::erase()
  {
    m_deleted = true;
  }

  bool Entity::isErased() const
  {
    return m_deleted;
  }

  void Entity::baseUpdate(const float dt)
  {
    if (isActive()) {
      update(dt);
    }
  }

  void Entity::baseDraw(sf::RenderTarget& target) const
  {
    if (isActive()) {
      draw(target);
    }
  }

  void Entity::update(const float dt)
  {}

  void Entity::draw(sf::RenderTarget& target) const
  {}

  void Entity::setActive(const bool active)
  {
    m_active = active;
  }

  bool Entity::isActive() const
  {
    return m_active;
  }

  const std::string& Entity::getName() const
  {
    return m_name;
  }
}
