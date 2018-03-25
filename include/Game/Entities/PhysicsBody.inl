template<typename T>
template<typename ... Args>
PhysicsBody<T>::PhysicsBody(const std::string& name, const sf::Vector2f& initialPos, const float initialRot, Args&&... args)
  : wtf::Drawable<T>(name, args...)
  , m_shape(nullptr)
  , m_body(nullptr)
  , m_lastPosition(initialPos.x, initialPos.y)
  , m_currentPosition(m_lastPosition)
  , m_lastRotation(initialRot)
  , m_currentRotation(m_lastRotation)
  , m_previousStep(1.f / 60)
  , m_accum(0.f)
{
  setPosition(initialPos);
  setRotation(initialRot);
}

template<typename T>
PhysicsBody<T>::~PhysicsBody()
{
  cpSpaceRemoveBody(cpBodyGetSpace(m_body), m_body);
  if (m_body) {
    cpBodyFree(m_body);
  }
  if (m_shape) {
    cpShapeFree(m_shape);
  }
}

template<typename T>
void PhysicsBody<T>::update(const float dt)
{
  m_accum += dt;

  const auto alpha = m_accum / m_previousStep;
  const auto pos = glm::mix(m_lastPosition, m_currentPosition, alpha);
  const auto rot = glm::mix(m_lastRotation, m_currentRotation, alpha);
  std::cout << pos.x << ", " << pos.y << std::endl;

  setPosition(pos.x, pos.y);
  setRotation(rot);
}

template<typename T>
void PhysicsBody<T>::fixedUpdate(const float step)
{
  m_previousStep = step;
  m_accum = 0.f;

  m_lastPosition = glm::vec2(getPosition().x, getPosition().y);
  m_lastRotation = getRotation();

  const auto currentPos = cpBodyGetPosition(m_body);
  m_currentPosition = glm::vec2(currentPos.x, currentPos.y);

  m_currentRotation = glm::degrees(cpBodyGetAngle(m_body));
}
