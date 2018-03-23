#include <WTF/System/Network.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

namespace wtf
{
  sf::TcpSocket ns_tcp;
  bool ns_tcpConnected = false;
  Network::MessageFunc ns_callback;

  bool Network::connectTcp(const std::string& address, const unsigned short port)
  {
    ns_tcp.setBlocking(true);
    ns_tcp.disconnect();

    ns_tcpConnected = ns_tcp.connect(address, port, sf::seconds(5.f)) == sf::Socket::Status::Done;

    if (ns_tcpConnected) {
      ns_tcp.setBlocking(false);
    }

    return ns_tcpConnected;
  }

  void Network::pollMessages()
  {
    if (!ns_callback) {
      return;
    }

    static std::string buffer(USHRT_MAX, '\0');
    static size_t tcpCursor = 0;
    std::size_t received = 0;

    while (true) {
      const auto result = ns_tcp.receive(&buffer[tcpCursor], buffer.size(), received);

      switch (result)
      {
        case sf::Socket::Status::Done:
        case sf::Socket::Status::Partial:
          break;

        case sf::Socket::Status::NotReady:
        case sf::Socket::Status::Error:
          return;

        case sf::Socket::Status::Disconnected:
          ns_tcpConnected = false;
          return ns_callback("disconnect");
      }

      const auto pos = buffer.find(";end;", tcpCursor);
      tcpCursor += received;

      if (pos != std::string::npos) {
        ns_callback(buffer.substr(0, pos));

        const auto sub = buffer.substr(pos + 5, tcpCursor - received + pos + 5);
        memcpy(&buffer[0], sub.c_str(), sub.size());

        tcpCursor -= pos + 5;
      }
    }
  }

  void Network::setOnMessage(MessageFunc && func)
  {
    ns_callback = func;
  }

  bool Network::sendMessage(const std::string& data, const SocketType type)
  {
    size_t sentTotal = 0;

    while (true) {
      size_t sent = 0;
      const auto status = ns_tcp.send(&data[sentTotal], data.size() - sentTotal, sent);

      switch (status)
      {
        case sf::Socket::Status::Done:
          return true;

        case sf::Socket::Status::NotReady:
          continue;

        case sf::Socket::Status::Partial:
          sentTotal += sent;
          continue;

        case sf::Socket::Disconnected:
          ns_tcpConnected = false;

          if (ns_callback) {
            ns_callback("disconnect");
          }

          return false;
      };
    }
  }
}
