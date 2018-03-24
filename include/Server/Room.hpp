#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace sf
{
  class TcpSocket;
}

namespace usbs
{
  class Room final
  {
  public:

    enum class Status
    {
      Waiting,
      Running,
      Ending,
      Ended,
    };

  private:

    typedef std::unordered_map<std::string, std::unique_ptr<sf::TcpSocket>> ClientMap;

  public:

    Room();

    ~Room();

    void forceExit();

    bool join(const std::string& id, std::unique_ptr<sf::TcpSocket>&& client);

    Status getStatus() const;

  private:

    void operator ()();

  private:

    std::atomic<Status> m_status;
    ClientMap m_pendingClients;
    ClientMap m_clients;
    std::mutex m_clientJoinMutex;
    std::thread m_thread;
  };
}
