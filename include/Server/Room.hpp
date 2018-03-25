#pragma once

#include <Server/Level/Level.hpp>
#include <atomic>
#include <string>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace sf
{
  class Packet;
}

namespace usbs
{
  class Socket;

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

    typedef std::unordered_map<std::string, std::unique_ptr<Socket>> ClientMap;

  public:

    Room();

    ~Room();

    void forceExit();

    bool join(const std::string& id, std::unique_ptr<Socket>&& client);

    Status getStatus() const;

  private:

    void operator ()();

    void emit(const sf::Packet& packet, const char* except = nullptr);

    void onBegin();

  private:

    std::atomic<Status> m_status;
    std::unique_ptr<Level> m_level;
    ClientMap m_pendingClients;
    ClientMap m_clients;
    std::mutex m_clientJoinMutex;
    std::thread m_thread;
  };
}
