#pragma once

#include <WTF/Scene/Entity.hpp>
#include <chipmunk/chipmunk_structs.h>
#include <glm/vec2.hpp>

namespace usb
{
  class Wall : public wtf::Entity
  {
  public:

    enum Which
    {
      Left,
      Right,
      Ceiling,
      Ground,
    };

  public:

    Wall(const std::string& name, const glm::vec2& levelSize, const Which which, cpSpace* space);

    ~Wall() override;

  private:

    cpShape* m_shape;
  };
}
