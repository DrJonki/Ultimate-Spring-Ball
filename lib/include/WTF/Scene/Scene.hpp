#pragma once

#include <SFML/Graphics/View.hpp>
#include <rapidjson/document.h>
#include <map>
#include <memory>

namespace sf
{
  class RenderTarget;
}

namespace wtf
{
  class Layer;

  class Scene
  {
  public:

    Scene();

    virtual ~Scene() = 0;

    Layer& addLayer(const uint32_t weight);

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

    sf::View& getView();

    const sf::View& getView() const;

  private:

    std::multimap<uint32_t, std::unique_ptr<Layer>> m_layers;
    sf::View m_view;
  };
}
