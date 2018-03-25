#pragma once

#include <chipmunk/chipmunk_structs.h>

namespace usbs
{
  class Socket;

  class Player
  {
  public:

    Player(cpSpace* space);

    virtual ~Player();

  private:

    cpBody * m_body;
    cpShape * m_shape;
  };
}
