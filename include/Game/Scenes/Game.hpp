#pragma once

#include <Game/Scenes/Base.hpp>
#include <chipmunk/chipmunk_structs.h>
#include <SFML/Audio/Music.hpp>
#include <memory>
#include <thread>

namespace usbs
{
  class Server;
}

namespace usb
{
  class Player;

  class GameScene : public BaseScene
  {
  public:

    GameScene(const bool local);

    ~GameScene();

  private:

    void update(const float dt) override;

    void fixedUpdate(const float step) override;

    void onWindowEvent(const sf::Event& event) override;

    void onSocketMessage(const std::string& message, sf::Packet& packet) override;

  private:

    std::unique_ptr<usbs::Server> m_localServer;
    std::thread m_localServerThread;
    cpSpace* m_space;
    const bool m_local;

    // Layers
    wtf::Layer& m_worldLayer;

    // Player
    Player& m_player;

    // Views
    sf::View m_worldView;

    // Music
    sf::Music m_music;
  };
}
