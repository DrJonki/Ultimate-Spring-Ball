#pragma once

#include <WTF/Scene/Drawable.hpp>
#include <WTF/Scene/Entity.hpp>
#include <chipmunk/chipmunk.h>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <iostream>

namespace usb
{
  template<typename T>
  class PhysicsBody : public wtf::Drawable<T>
  {
  public:

    template<typename ... Args>
    PhysicsBody(const std::string& name, const sf::Vector2f& initialPos, const float initialRot, Args&&... args);

    ~PhysicsBody() override;

  private:

    void update(const float dt) override;

    void fixedUpdate(const float step) override;

  protected:

    cpShape * m_shape;
    cpBody* m_body;

  private:

    glm::vec2 m_lastPosition;
    glm::vec2 m_currentPosition;
    float m_lastRotation;
    float m_currentRotation;
    float m_previousStep;
    float m_accum;
  };

  #include <Game/Entities/PhysicsBody.inl>
}