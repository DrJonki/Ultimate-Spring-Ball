#include <WTF/Scene/Component.hpp>

namespace wtf
{
  Component::Component(Entity& entity)
    : m_entity(entity)
    , m_active(true)
  {}

  Component::~Component()
  {}

  void Component::setActive(const bool active)
  {
    m_active = active;
  }

  bool Component::isActive() const
  {
    return m_active;
  }

  Entity & Component::getEntity()
  {
    return m_entity;
  }

  const Entity & Component::getEntity() const
  {
    return m_entity;
  }

  void Component::baseUpdate(const float delta)
  {
    if (isActive()) {
      return update(delta);
    }
  }

  void Component::baseFixedUpdate(const float step)
  {
    if (isActive()) {
      return fixedUpdate(step);
    }
  }

  void Component::update(const float)
  {}

  void Component::fixedUpdate(const float base)
  {}
}
