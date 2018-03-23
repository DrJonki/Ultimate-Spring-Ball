#include <WTF/System/Config.hpp>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <unordered_map>

namespace
{
  std::unordered_map<std::string, std::string> m_config;
}

namespace wtf
{
  void Config::ensureInit()
  {
    static bool init = false;

    if (init)
      return;

    init = true;

    std::ifstream file("assets/config.txt");
    assert(file.good());

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream is_line(line);
      std::string key;

      if (std::getline(is_line, key, '=') && key[0] != '#') {
        std::string value;
        if (std::getline(is_line, value))
          m_config[key] = value;
      }
    }
  }

  template<>
  std::string Config::get(const std::string& key)
  {
    ensureInit();

    auto itr = m_config.find(key);
    assert(itr != m_config.end());

    return itr->second;
  }

  template<>
  int Config::get(const std::string& key)
  {
    return std::atoi(get<std::string>(key).c_str());
  }

  template<>
  float Config::get(const std::string& key)
  {
    return static_cast<float>(std::atof(get<std::string>(key).c_str()));
  }

  template<>
  bool Config::get(const std::string& key)
  {
    return get<std::string>(key) == "true";
  }
}
