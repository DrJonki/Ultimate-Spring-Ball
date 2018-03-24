#pragma once

#include <WTF/Scene/Drawable.hpp>
#include <WTF/Scene/Entity.hpp>
#include <chipmunk/chipmunk.h>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>

namespace usb
{
  template<typename T>
  class PhysicsBody : public wtf::Drawable<T>
  {
  public:

    struct Attributes
    {
      cpFloat mass;
    };

  public:

    template<typename ... Args>
    PhysicsBody(cpSpace& space, const Attributes& attribs, Args&&... args);

    ~PhysicsBody() override;

  private:

    void update(const float dt) override;

    void fixedUpdate(const float step) override;

  private:

    cpBody* m_body;
    cpSpace* m_space;
    glm::vec2 m_lastPosition;
    glm::vec2 m_currentPosition;
    float m_lastRotation;
    float m_currentRotation;
    float m_previousStep;
    float m_accum;
  };
}