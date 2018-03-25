#include <Server/Level/Level.hpp>
#include <Server/Room.hpp>
#include <WTF/Util/Randomizer.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <cmath>

namespace usbs
{
  Level::Level(Room& room)
    : m_room(room)
    , m_obstacles()
    , m_levelJson()
    , m_space(cpSpaceNew())
  {
    auto hsvToRgb = [](const float h, const float s, const float v) -> sf::Color {
      const auto h_i = static_cast<int>(h * 6);
      const auto f = h * 6 - h_i;
      const auto p = v * (1 - s);
      const auto q = v * (1 - f * s);
      const auto t = v * (1 - (1 - f) * s);

      switch (h_i)
      {
        case 0: return sf::Color(static_cast<sf::Uint8>(v), static_cast<sf::Uint8>(t), static_cast<sf::Uint8>(p));
        case 1: return sf::Color(static_cast<sf::Uint8>(q), static_cast<sf::Uint8>(v), static_cast<sf::Uint8>(p));
        case 2: return sf::Color(static_cast<sf::Uint8>(p), static_cast<sf::Uint8>(v), static_cast<sf::Uint8>(t));
        case 3: return sf::Color(static_cast<sf::Uint8>(p), static_cast<sf::Uint8>(q), static_cast<sf::Uint8>(v));
        case 4: return sf::Color(static_cast<sf::Uint8>(t), static_cast<sf::Uint8>(p), static_cast<sf::Uint8>(v));
        case 5: return sf::Color(static_cast<sf::Uint8>(v), static_cast<sf::Uint8>(p), static_cast<sf::Uint8>(q));
        default: return sf::Color();
      }
    };

    static wtf::Randomizer colorRand;
    auto colorRandCurrent = colorRand(0.f, 1.f);
    auto generateColor = [&colorRandCurrent, hsvToRgb] {
      colorRandCurrent += static_cast<float>(0.618033988749895);
      colorRandCurrent = std::fmod(colorRandCurrent, 1.f);

      return hsvToRgb(colorRandCurrent, 0.5f, 0.95f);
    };

    cpSpaceSetGravity(m_space, cpv(0.f, 10.f));

    const glm::vec2 levelSize(250, 250);

    // Generate
    {
      // Walls
      {
        // Left
        {
          Object obj;

          auto shape = obj.shape = cpSegmentShapeNew(cpSpaceGetStaticBody(m_space), cpv(0, 0), cpv(0, levelSize.y), 0);
          cpSpaceAddShape(m_space, shape);

          m_obstacles.push_back(obj);
        }

        // Right
        {
          Object obj;

          auto shape = obj.shape = cpSegmentShapeNew(cpSpaceGetStaticBody(m_space), cpv(levelSize.x, 0), cpv(levelSize.x, levelSize.y), 0);
          cpSpaceAddShape(m_space, shape);

          m_obstacles.push_back(obj);
        }

        // Ceiling
        {
          Object obj;

          auto shape = obj.shape = cpSegmentShapeNew(cpSpaceGetStaticBody(m_space), cpv(0, 0), cpv(levelSize.x, 0), 0);
          cpSpaceAddShape(m_space, shape);

          m_obstacles.push_back(obj);
        }
      }

      // Ground
      {
        Object obj;

        auto shape = obj.shape = cpSegmentShapeNew(cpSpaceGetStaticBody(m_space), cpv(0, levelSize.y), cpv(levelSize.x, levelSize.y), 0);
        cpShapeSetCollisionType(shape, ObstacleType::Ground);

        m_obstacles.push_back(obj);

        auto handler = cpSpaceAddWildcardHandler(m_space, ObstacleType::Ground);

        handler->userData = this;
        handler->beginFunc = [](cpArbiter *arb, struct cpSpace *space, cpDataPointer data) -> cpBool {


          return true;
        };
      }

      // Obstacles
      {
        wtf::Randomizer rand;
        unsigned int budget = 20;

        while (budget-- > 0) {
          const int type = rand.range(ObstacleType::ObstacleRect + 0, ObstacleType::Count - 1);

          switch (type)
          {
            case ObstacleType::ObstacleRect: {

              break;
            }
            case ObstacleType::ObstacleCircle: {

              break;
            }
          }
        }
      }
    }
  }

  Level::~Level()
  {
    for (auto& i : m_obstacles) {
      if (i.shape) {
        cpShapeFree(i.shape);
      }
      if (i.body) {
        cpBodyFree(i.body);
      }
    }

    cpSpaceFree(m_space);
  }

  void Level::update(const float dt)
  {
    cpSpaceStep(m_space, dt);
  }

  std::string Level::getJson() const
  {
    return m_levelJson;
  }
  Room & Level::getRoom()
  {
    return m_room;
  }
}
