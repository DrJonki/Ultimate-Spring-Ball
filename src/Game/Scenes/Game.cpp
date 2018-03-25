#include <Game/Scenes/Game.hpp>
#include <Game/Entities/Obstacle.hpp>
#include <Game/Entities/Player.hpp>
#include <Game/Entities/Wall.hpp>
#include <WTF/Scene/Layer.hpp>
#include <WTF/System/Network.hpp>
#include <WTF/System/Engine.hpp>
#include <Server/Server.hpp>
#include <SFML/Network/Packet.hpp>
#include <rapidjson/pointer.h>
#include <chipmunk/chipmunk.h>

namespace usb
{
  GameScene::GameScene(const bool local)
    : BaseScene()
    , m_localServer()
    , m_localServerThread()
    , m_space(cpSpaceNew())
    , m_local(local)
    , m_worldLayer(addLayer(10))
    , m_player(m_worldLayer.insert<Player>("player", m_space))
    , m_worldView()
    , m_music()
  {
    // Music
    {
      m_music.openFromFile("assets/Music/Blip Stream.ogg");
      m_music.setRelativeToListener(true);
      m_music.setLoop(true);
      m_music.setVolume(60.f);
      m_music.play();
    }

    m_worldLayer.setView(&m_worldView);

    cpSpaceSetGravity(m_space, cpv(0, 20.f));

    if (local) {
      m_localServer = std::make_unique<usbs::Server>(true);
      auto localServerPtr = m_localServer.get();

      m_localServerThread = std::thread([localServerPtr] {
        (*localServerPtr)(12000);
      });

      wtf::Network::disconnectTcp();

      while (!wtf::Network::connectTcp("127.0.0.1", 12000));

      sf::Packet packet;
      packet << "joinRandomGame";

      wtf::Network::sendMessage(packet);
    }
  }

  GameScene::~GameScene()
  {
    clearLayers();

    cpSpaceFree(m_space);

    m_localServer->stop();
    m_localServerThread.join();
  }

  void GameScene::update(const float dt)
  {

    BaseScene::update(dt);
  }

  void GameScene::fixedUpdate(const float step)
  {
    cpSpaceStep(m_space, step);

    BaseScene::fixedUpdate(step);
  }

  void GameScene::onWindowEvent(const sf::Event& event)
  {
    switch (event.type)
    {
      case sf::Event::EventType::MouseButtonPressed: {
        const sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
        m_player.onMouseClick(wtf::Engine::getWindow().mapPixelToCoords(pos, m_worldView));
        break;
      }
    }
  }

  void GameScene::onSocketMessage(const std::string& message, sf::Packet& packet)
  {
    if (message == "joinedSelf") {

    }
    else if (message == "joined") {

    }
    else if (message == "roundBegin") {
      std::string levelJson;
      packet >> levelJson;

      namespace rj = rapidjson;

      rj::Document doc;
      doc.ParseInsitu(&levelJson[0]);

      const glm::vec2 levelSize(
        rj::Pointer("/levelSize/0").Get(doc)->GetFloat(),
        rj::Pointer("/levelSize/1").Get(doc)->GetFloat()
      );

      m_worldView.setCenter(levelSize.x / 2, levelSize.y / 2);
      m_worldView.setSize(levelSize.x, levelSize.y);

      for (int i = 0; i < 4; ++i) {
        m_worldLayer.insert<Wall>("wall-" + std::to_string(i), levelSize, static_cast<Wall::Which>(i), m_space);
      }

      auto obsArray = rj::Pointer("/obstacles").Get(doc);
      for (auto itr = obsArray->Begin(); itr != obsArray->End(); ++itr) {
        m_worldLayer.insert<Obstacle>(rj::Pointer("/id").Get(*itr)->GetString(), *itr, m_space);
      }
    }
    else if (message == "simulationBegin") {

    }
  }
}
