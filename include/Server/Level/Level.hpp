#pragma once

#include <chipmunk/chipmunk_structs.h>
#include <SFML/Graphics/Color.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

namespace usbs
{
  class Room;

  class Level
  {
  public:

    enum ObstacleType
    {
      Ground,
      Player,

      ObstacleRect,
      ObstacleCircle,
      Count,
    };

  private:

    struct Object
    {
      sf::Color color;
      cpShape* shape = nullptr;
    };

  public:

    Level(Room& room);

    ~Level();

    void update(const float dt);

    std::string getJson() const;

    Room& getRoom();

  private:

    Room & m_room;
    std::vector<Object> m_obstacles;
    std::string m_levelJson;
    cpSpace* m_space;
  };
}
