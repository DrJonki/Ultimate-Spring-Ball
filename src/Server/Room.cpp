#include <Server/Room.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <algorithm>

namespace usbs
{
  Room::Room()
    : m_status(Status::Waiting)
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

  bool Room::join(const std::string& id, std::unique_ptr<sf::TcpSocket>&& client)
  {
    if (getStatus() != Status::Waiting) {
      return false;
    }

    std::lock_guard<std::mutex> lock(m_clientJoinMutex);

    m_pendingClients[id] = std::move(client);

    return true;
  }

  void Room::operator ()()
  {
    static const int tickRate = 60;
    static const float frameLenS = 1.f / tickRate;
    static const long long frameLenUs = static_cast<long long>(frameLenS) * 1000 * 1000; // Microseconds

    sf::Clock clock;

    while (getStatus() != Status::Ended) {
      // Check for pending clients
      if (getStatus() == Status::Waiting)
      {
        std::lock_guard<std::mutex> lock(m_clientJoinMutex);
      }

      const auto elapsed = clock.restart().asMicroseconds();
      std::this_thread::sleep_for(std::chrono::microseconds(std::max(0ll, frameLenUs - elapsed)));
    }
  }

  Room::Status Room::getStatus() const
  {
    return m_status.load();
  }
}
