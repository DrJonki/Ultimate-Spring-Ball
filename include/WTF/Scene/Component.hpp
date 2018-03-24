#pragma once

#include <WTF/Export.hpp>

namespace wtf
{
  class Entity;

  class WTF_API Component
  {
    friend class Entity;

  public:

    Component(Entity& entity);

    virtual ~Component() = 0;

    void setActive(const bool active);

    bool isActive() const;

    Entity& getEntity();

    const Entity& getEntity() const;

  private:

    void baseUpdate(const float delta);

    void baseFixedUpdate(const float step);

    virtual void update(const float delta);

    virtual void fixedUpdate(const float base);

  private:

    Entity& m_entity;
    bool m_active;
  };
}
