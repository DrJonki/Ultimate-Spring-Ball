#include <Server/Player.hpp>
#include <chipmunk/chipmunk.h>
#include "..\..\include\Game\Entities\Player.hpp"

namespace usbs
{
  Player::Player(cpSpace * space)
    : m_body(cpSpaceAddBody(space, cpBodyNew(1.f, cpMomentForCircle(1.f, 0, 1.f, cpvzero))))
    , m_shape(cpSpaceAddShape(space, cpCircleShapeNew(m_body, 1.f, cpvzero)))
  {
    
  }

  Player::~Player()
  {
  }
}
