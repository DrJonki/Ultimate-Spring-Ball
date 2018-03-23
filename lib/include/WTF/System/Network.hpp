#pragma once

#include <WTF/Macros.hpp>
#include <functional>
#include <string>

namespace wtf
{
  class Network
  {
    WTF_NO_COPY(Network);

  public:

    enum class SocketType
    {
      Tcp,
      Udp,
    };

    typedef std::function<void(const std::string& data)> MessageFunc;

  public:

    static bool connectTcp(const std::string& address, const unsigned short port);

    static void pollMessages();

    static void setOnMessage(MessageFunc&& func);

    static bool sendMessage(const std::string& data, const SocketType type = SocketType::Tcp);
  };
}
