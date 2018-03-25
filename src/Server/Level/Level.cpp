#include <Server/Level/Level.hpp>
#include <Server/Room.hpp>
#include <WTF/Util/Randomizer.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <glm/trigonometric.hpp>
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
        case 0: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255));
        case 1: return sf::Color(static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255));
        case 2: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255));
        case 3: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255));
        case 4: return sf::Color(static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255));
        case 5: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255));
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
    cpSpaceSetUserData(m_space, this);

    const glm::vec2 levelSize(250, 250);

    // Generate
    {
      namespace rj = rapidjson;

      rj::Document doc(rapidjson::kObjectType);

      // Add level size
      {
        rj::Value size(rj::kArrayType);
        size.PushBack(levelSize.x, doc.GetAllocator());
        size.PushBack(levelSize.y, doc.GetAllocator());

        doc.AddMember(rj::StringRef("levelSize"), std::move(size), doc.GetAllocator());
      }

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
        cpSpaceAddShape(m_space, shape);

        m_obstacles.push_back(obj);

        auto handler = cpSpaceAddCollisionHandler(m_space, ObstacleType::Ground, ObstacleType::Player);

        handler->beginFunc = [](cpArbiter *arb, struct cpSpace *space, cpDataPointer) -> cpBool {
          auto self = static_cast<Level*>(cpSpaceGetUserData(space));
          

          return true;
        };
      }

      // Obstacles
      {
        static unsigned long long obstacleId = 0;
        const float elasticity = 1.3f;
        wtf::Randomizer rand;
        int budget = 20;

        rj::Value obstaclesJson(rj::kArrayType);

        while (budget-- > 0) {
          const int type = rand.range(ObstacleType::ObstacleRect + 0, ObstacleType::Count - 1);
          Object obj;

          rj::Value shapeJson(rj::kObjectType);

          switch (type)
          {
            case ObstacleType::ObstacleRect: {
              auto shape = obj.shape = cpBoxShapeNew(cpSpaceGetStaticBody(m_space), 5.f, 5.f, 0.5f);

              cpShapeSetElasticity(shape, elasticity);

              rj::Value sizeJson(rj::kArrayType);
              sizeJson.PushBack(5.f, doc.GetAllocator());
              sizeJson.PushBack(5.f, doc.GetAllocator());

              shapeJson.AddMember(rj::StringRef("size"), std::move(sizeJson), doc.GetAllocator());
              shapeJson.AddMember(rj::StringRef("radius"), 0.5f, doc.GetAllocator());

              break;
            }
            case ObstacleType::ObstacleCircle: {
              auto shape = obj.shape = cpCircleShapeNew(cpSpaceGetStaticBody(m_space), 2.5f, cpvzero);

              cpShapeSetElasticity(shape, elasticity);

              shapeJson.AddMember("radius", 2.5f, doc.GetAllocator());

              break;
            }
          }

          auto body = cpShapeGetBody(obj.shape);
          const auto bodyPos = cpv(rand.range(10.f, levelSize.x - 10.f), rand.range(levelSize.y * 0.2f, levelSize.y - 15.f));
          const auto bodyAngle = glm::radians(rand.range(0.f, 359.f));
          cpBodySetPosition(body, bodyPos);
          cpBodySetAngle(body, bodyAngle);

          obj.color = generateColor();
          cpSpaceAddShape(m_space, obj.shape);

          // Json
          {
            rj::Value val(rj::kObjectType);

            val.AddMember(rj::StringRef("id"), rj::Value(std::to_string(++obstacleId).c_str(), doc.GetAllocator()), doc.GetAllocator());
            val.AddMember(rj::StringRef("type"), type, doc.GetAllocator());
            val.AddMember(rj::StringRef("elasticity"), cpShapeGetElasticity(obj.shape), doc.GetAllocator());
            val.AddMember(rj::StringRef("angle"), glm::degrees(bodyAngle), doc.GetAllocator());
            val.AddMember(rj::StringRef("shape"), std::move(shapeJson), doc.GetAllocator());

            rj::Value pos(rj::kArrayType);
            pos.PushBack(bodyPos.x, doc.GetAllocator());
            pos.PushBack(bodyPos.y, doc.GetAllocator());
            val.AddMember(rj::StringRef("position"), std::move(pos), doc.GetAllocator());

            rj::Value color(rj::kArrayType);
            color.PushBack(obj.color.r, doc.GetAllocator());
            color.PushBack(obj.color.g, doc.GetAllocator());
            color.PushBack(obj.color.b, doc.GetAllocator());
            color.PushBack(obj.color.a, doc.GetAllocator());
            val.AddMember(rj::StringRef("color"), std::move(color), doc.GetAllocator());

            obstaclesJson.PushBack(std::move(val), doc.GetAllocator());
          }

          m_obstacles.push_back(obj);
        }

        doc.AddMember("obstacles", std::move(obstaclesJson), doc.GetAllocator());
      }

      rj::StringBuffer buf;
      rj::Writer<rj::StringBuffer> writer(buf);
      doc.Accept(writer);

      m_levelJson = std::string(buf.GetString(), buf.GetLength());
    }
  }

  Level::~Level()
  {
    for (auto& i : m_obstacles) {
      if (i.shape) {
        cpShapeFree(i.shape);
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
