#pragma once

#include <WTF/Export.hpp>
#include <WTF/Macros.hpp>
#include <SFML/Network/Packet.hpp>
#include <functional>
#include <string>

namespace wtf
{
  class WTF_API Network
  {
    WTF_NO_COPY(Network);

  public:

    enum class SocketType
    {
      Tcp,
      Udp,
    };

    typedef std::function<void(sf::Packet&)> MessageFunc;

  public:

    static bool connectTcp(const std::string& address, const unsigned short port);

    static void disconnectTcp();

    static bool tcpConnected();

    static void pollMessages();

    static void setOnMessage(MessageFunc&& func);

    static bool sendMessage(sf::Packet& packet);
  };
}
