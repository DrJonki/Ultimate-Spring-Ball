#include <Server/Server.hpp>
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

              if (selector.isReady(client)) {
                sf::Packet packet;

                if (client.receive(packet) == sf::Socket::Status::Done) {
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
  }

  void Server::handleConnection(std::unique_ptr<sf::TcpSocket>&& client)
  {
    m_clients[client->getRemoteAddress().toString() + ":" + std::to_string(client->getRemotePort())] = std::move(client);
  }

  void Server::handleMessage(sf::Packet& packet, const std::string& clientId)
  {
    std::string event;
    packet >> event;

    if (event == "findGame") {

    }
  }
}
