#pragma once

#include <memory>

namespace sf
{
  class TcpSocket;
  class Packet;
}

namespace usbs
{
  class Socket final
  {
  public:

    Socket(std::unique_ptr<sf::TcpSocket>&& socket);

    ~Socket();

    bool send(sf::Packet& packet);

    sf::TcpSocket& get();

  private:

    std::unique_ptr<sf::TcpSocket> m_socket;
  };
}
