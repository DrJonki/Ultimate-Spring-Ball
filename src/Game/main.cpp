#include <WTF/System/Engine.hpp>
#include <Game/Scenes/Connecting.hpp>

int main() {
  wtf::Engine eng;

  eng.pushScene<usb::ConnectingScene>();
  eng();

  return 0;
}
