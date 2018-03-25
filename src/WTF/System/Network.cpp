#include <WTF/System/Network.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <atomic>

namespace wtf
{
  sf::TcpSocket ns_tcp;
  std::atomic<bool> ns_tcpConnected(false);
  Network::MessageFunc ns_callback;

  bool Network::connectTcp(const std::string& address, const unsigned short port)
  {
    if (tcpConnected()) {
      return true;
    }

    ns_tcpConnected.store(ns_tcp.connect(address, port, sf::seconds(2.f)) == sf::Socket::Status::Done);

    if (tcpConnected()) {
      ns_tcp.setBlocking(false);
      return true;
    }

    return false;
  }

  void Network::disconnectTcp()
  {
    ns_tcp.disconnect();
    ns_tcpConnected.store(false);
  }

  bool Network::tcpConnected()
  {
    return ns_tcpConnected.load();
  }

  void Network::pollMessages()
  {
    if (!ns_callback || !tcpConnected()) {
      return;
    }

    sf::Packet packet;

    while (true) {
      switch (ns_tcp.receive(packet))
      {
        case sf::Socket::Status::Done:
          ns_callback(packet);
          break;

        case sf::Socket::Status::Partial:
          break;

        case sf::Socket::Status::NotReady:
        case sf::Socket::Status::Error:
          return;

        case sf::Socket::Status::Disconnected:
          disconnectTcp();

          sf::Packet disconnectPacket;
          disconnectPacket << "disconnect" << "connection interrupted";

          return ns_callback(disconnectPacket);
      }
    }
  }

  void Network::setOnMessage(MessageFunc && func)
  {
    ns_callback = func;
  }

  bool Network::sendMessage(sf::Packet& packet)
  {
    while (true) {
      switch (ns_tcp.send(packet))
      {
        case sf::Socket::Status::Done:
          return true;

        case sf::Socket::Status::NotReady:
          continue;

        case sf::Socket::Status::Partial:
          continue;

        case sf::Socket::Disconnected:
          disconnectTcp();

          sf::Packet disconnectPacket;
          disconnectPacket << "disconnect" << "connection interrupted";

          if (ns_callback) {
            ns_callback(disconnectPacket);
          }

          return false;
      };
    }
  }
}
