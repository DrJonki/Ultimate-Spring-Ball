#include <WTF/Scene/Entity.hpp>
#include <WTF/Scene/Component.hpp>

namespace wtf
{
  Entity::Entity(const std::string& name)
    : m_components  ()
    , m_name        (name)
    , m_active      (true)
    , m_deleted     (false)
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
      for (auto& i : m_components) {
        i.second->baseUpdate(dt);
      }

      return update(dt);
    }
  }

  void Entity::baseFixedUpdate(const float step)
  {
    if (isActive()) {
      for (auto& i : m_components) {
        i.second->baseFixedUpdate(step);
      }

      return fixedUpdate(step);
    }
  }

  void Entity::baseDraw(sf::RenderTarget& target) const
  {
    if (isActive()) {
      return draw(target);
    }
  }

  void Entity::update(const float dt)
  {}

  void Entity::fixedUpdate(const float step)
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
