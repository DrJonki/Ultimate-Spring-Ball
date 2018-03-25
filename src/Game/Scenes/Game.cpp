#include <Game/Scenes/Game.hpp>
#include <Server/Server.hpp>
#include <chipmunk/chipmunk.h>

namespace usb
{
  GameScene::GameScene(const bool local)
    : m_localServer()
    , m_localServerThread()
    , m_space(cpSpaceNew())
    , m_local(local)
  {
    cpSpaceSetGravity(m_space, cpv(0, -10.f));

    if (local) {
      m_localServer = std::make_unique<usbs::Server>(true);
      auto localServerPtr = m_localServer.get();

      m_localServerThread = std::thread([localServerPtr] {
        (*localServerPtr)(12000);
      });
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

  }

  void GameScene::onSocketMessage(const std::string& message, sf::Packet& packet)
  {

  }
}
