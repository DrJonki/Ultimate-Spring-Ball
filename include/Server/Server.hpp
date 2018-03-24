#pragma once

#include <Server/Room.hpp>
#include <atomic>
#include <memory>
#include <unordered_map>

namespace sf
{
  class Packet;
  class TcpSocket;
}

namespace usbs
{
  class Server final
  {
  public:

    Server(const bool local);

    ~Server();

    int operator ()(const unsigned short port);

    void stop();

    void emit(sf::Packet& packet);

  private:

    void handleConnection(std::unique_ptr<sf::TcpSocket>&& client);

    void handleMessage(sf::Packet& packet, const std::string& clientId);

  private:

    const bool m_local;
    std::atomic<bool> m_running;
    std::unordered_map<unsigned long long, std::unique_ptr<Room>> m_rooms;
    std::unordered_map<std::string, std::unique_ptr<sf::TcpSocket>> m_clients;
  };
}
