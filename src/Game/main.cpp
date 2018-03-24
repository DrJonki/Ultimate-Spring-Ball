#include <WTF/System/Engine.hpp>
#include <Game/Scenes/Title.hpp>
#include <Server/Server.hpp>
#include <cstring>
#include <string>

int main(int argc, char* argv[]) {
  bool server = false;
  unsigned short port = 12000;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--port") == 0) {
      port = static_cast<unsigned short>(std::stoul(argv[i + 1]));
      ++i;
    }
    else if (strcmp(argv[i], "--server") == 0) {
      server = true;
    }
  }

  if (server) {
    usbs::Server server(false);

    return server(port);
  }
  else {
    wtf::Engine eng("Ultimate Spring Jump");

    eng.pushScene<usb::TitleScene>();
    return eng();
  }
}
