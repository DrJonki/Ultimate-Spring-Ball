#include <Game/Entities/Wall.hpp>

namespace usb
{
  Wall::Wall(const std::string& name, const glm::vec2& levelSize, const Which which, cpSpace* space)
    : wtf::Entity(name)
    , m_shape()
  {
    cpVect v1, v2;

    switch (which)
    {
      case Which::Ceiling:
        v1 = cpv(0, 0);
        v2 = cpv(levelSize.x, 0);
        break;

      case Which::Left:
        v1 = cpv(0, 0);
        v2 = cpv(0, levelSize.y);

        break;

      case Which::Right:
        v1 = cpv(levelSize.x, 0);
        v2 = cpv(levelSize.x, levelSize.y);

        break;

      case Which::Ground:
        v1 = cpv(0, levelSize.y);
        v2 = cpv(levelSize.x, levelSize.y);

        break;
    }

    m_shape = cpSpaceAddShape(space, cpSegmentShapeNew(cpSpaceGetStaticBody(space), v1, v2, 2));
    cpShapeSetElasticity(m_shape, 1.f);
  }

  Wall::~Wall()
  {
    cpShapeFree(m_shape);
  }
}
