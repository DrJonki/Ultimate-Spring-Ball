template<typename T>
template<typename ... Args>
PhysicsBody::PhysicsBody(cpSpace& space, const Attributes& attribs, Args&&... args)
  : wtf::Drawable(args...)
  , m_body(nullptr)
  , m_space(&space)
  , m_lastPosition(getPosition().x, getPosition().y)
  , m_currentPosition(m_lastPosition)
  , m_lastRotation(getEntity().getRotation())
  , m_currentRotation(m_lastRotation)
  , m_previousStep(0.f)
  , m_accum(0.f)
{

}

template<typename T>
PhysicsBody::~PhysicsBody()
{
  cpSpaceRemoveBody(m_space, m_body);
  cpBodyFree(m_body);
}

template<typename T>
void PhysicsBody::update(const float dt)
{
  m_accum += dt;

  const auto alpha = m_accum / m_previousStep;
  const auto pos = glm::mix(m_lastPosition, m_currentPosition, alpha);
  const auto rot = glm::mix(m_lastRotation, m_currentRotation, alpha);

  auto& e = getEntity();
  e.setPosition(pos.x, pos.y);
  e.setRotation(rot);
}

template<typename T>
void PhysicsBody::fixedUpdate(const float step)
{
  m_previousStep = step;
  m_accum = 0.f;

  const auto& e = getEntity();
  m_lastPosition = glm::vec2(e.getPosition().x, e.getPosition().y);
  m_lastRotation = e.getRotation();

  const auto currentPos = cpBodyGetPosition(m_body);
  m_currentPosition = glm::vec2(currentPos.x, currentPos.y);

  m_currentRotation = glm::degrees(cpBodyGetAngle(m_body));
}
