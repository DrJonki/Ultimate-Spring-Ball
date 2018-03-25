#include <Server/Room.hpp>
#include <Server/Socket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/Packet.hpp>
#include <algorithm>
#include <future>

namespace usbs
{
  Room::Room()
    : m_status(Status::Waiting)
    , m_level()
    , m_clients()
    , m_pendingClients()
    , m_clientJoinMutex()
    , m_thread([this] {
        (*this)();
      })
  {
    
  }

  Room::~Room()
  {
    forceExit();
    m_thread.join();
  }

  void Room::forceExit()
  {
    m_status.store(Status::Ended);
  }

  bool Room::join(const std::string& id, std::unique_ptr<Socket>&& client)
  {
    if (getStatus() != Status::Waiting) {
      return false;
    }

    auto ptr = client.release();
    std::async(std::launch::async, [this, id, ptr]() {
      std::lock_guard<std::mutex> lock(m_clientJoinMutex);

      m_pendingClients[id] = std::unique_ptr<Socket>(ptr);
    });

    return true;
  }

  void Room::operator ()()
  {
    static const int tickRate = 60;
    static const float frameLenS = 1.f / tickRate;
    static const long long frameLenUs = static_cast<long long>(frameLenS) * 1000 * 1000; // Microseconds

    sf::Clock clock;

    while (getStatus() != Status::Ended) {
      const auto status = getStatus();

      // Check for pending clients
      if (status == Status::Waiting)
      {
        std::lock_guard<std::mutex> lock(m_clientJoinMutex);

        for (auto& i : m_pendingClients) {
          sf::Packet packet;
          packet << "joined";

          /*for (auto& j : m_clients) {
            packet << j.first << 
          }*/

          i.second->send(packet);
        }
      }
      else if (status == Status::Running) {

      }

      const auto elapsed = clock.restart().asMicroseconds();
      std::this_thread::sleep_for(std::chrono::microseconds(std::max(0ll, frameLenUs - elapsed)));
    }

    // On close
    {
      sf::Packet packet;
      packet << "roomClosed";

      emit(packet);
    }
  }

  void Room::emit(const sf::Packet & packet, const char* except)
  {
    for (auto& i : m_clients) {
      if (!except || i.first != except) {
        sf::Packet packetCopy(packet);

        i.second->send(packetCopy);
      }
    }
  }

  void Room::onBegin()
  {
  }

  Room::Status Room::getStatus() const
  {
    return m_status.load();
  }
}
