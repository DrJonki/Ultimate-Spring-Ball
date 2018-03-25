#include <Server/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace usbs
{
  Socket::Socket(std::unique_ptr<sf::TcpSocket>&& socket)
    : m_socket(std::move(socket))
  {}

  Socket::~Socket()
  {}

  bool Socket::send(sf::Packet& packet)
  {
    while (true) {
      const auto status = m_socket->send(packet);

      if (status == sf::Socket::Status::Done) {
        break;
      }

      if (status == sf::Socket::Status::Partial) {
        continue;
      }

      return false;
    }

    return true;
  }

  sf::TcpSocket & Socket::get()
  {
    return *m_socket;
  }
}
