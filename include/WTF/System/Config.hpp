#pragma once

#include <WTF/Export.hpp>
#include <string>

namespace wtf
{
  class WTF_API Config
  {
  private:

    static void ensureInit();

  public:

    template<typename T>
    static T get(const std::string& key);
  };
}
