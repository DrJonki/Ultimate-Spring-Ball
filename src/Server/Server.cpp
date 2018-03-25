#include <Server/Server.hpp>
#include <Server/Socket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <iostream>

namespace usbs
{
  Server::Server(const bool local)
    : m_local(local)
    , m_running(true)
    , m_rooms()
    , m_clients()
  {

  }

  Server::~Server()
  {}

  int Server::operator ()(const unsigned short port)
  {
    try {
      sf::TcpListener listener;
      listener.listen(port);

      sf::SocketSelector selector;
      selector.add(listener);

      while (m_running.load()) {
        if (selector.wait(sf::milliseconds(500))) {
          if (selector.isReady(listener)) {
            auto newClient = std::make_unique<sf::TcpSocket>();

            if (listener.accept(*newClient) == sf::Socket::Status::Done) {
              selector.add(*newClient);

              handleConnection(std::move(newClient));
            }
          }
          else {
            for (auto& i : m_clients) {
              auto& client = *i.second;

              if (selector.isReady(client.get())) {
                sf::Packet packet;

                if (client.get().receive(packet) == sf::Socket::Status::Done) {
                  handleMessage(packet, i.first);
                }
              }
            }
          }
        }

        for (auto itr = m_rooms.begin(); itr != m_rooms.end();) {
          if (itr->second->getStatus() == Room::Status::Ended) {
            itr = m_rooms.erase(itr);
            continue;
          }

          ++itr;
        }
      }
    }
    catch (std::exception& ex) {
      sf::Packet packet;
      packet << "disconnect" << "server crashed";

      emit(packet);

      throw ex;
    }

    {
      sf::Packet packet;
      packet << "disconnect" << "server closed";

      emit(packet);
    }

    return 0;
  }

  void Server::stop()
  {
    m_running.store(false);
  }

  void Server::emit(sf::Packet & packet)
  {
    for (auto& i : m_clients) {
      sf::Packet packetCopy(packet);
      i.second->send(packetCopy);
    }
  }

  void Server::handleConnection(std::unique_ptr<sf::TcpSocket>&& client)
  {
    m_clients[client->getRemoteAddress().toString() + ":" + std::to_string(client->getRemotePort())] = std::make_unique<Socket>(std::move(client));
  }

  void Server::handleMessage(sf::Packet& packet, const std::string& clientId)
  {
    std::string event;
    packet >> event;

    if (event == "joinRandomGame") {
      const auto client = m_clients.find(clientId);

      for (auto& i : m_rooms) {
        if (i.second->join(clientId, std::move(client->second))) {
          m_clients.erase(client);
          return;
        }
      }

      static unsigned long long roomId = 0ul;

      auto newRoom = std::make_unique<Room>();
      newRoom->join(clientId, std::move(client->second));

      m_rooms[roomId++] = std::move(newRoom);

      return;
    }


  }
}
