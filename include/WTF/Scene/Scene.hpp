#pragma once

#include <WTF/Export.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
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

  class WTF_API Scene
  {
    friend class Engine;

  public:

    Scene();

    virtual ~Scene() = 0;

    Layer& addLayer(const uint32_t weight);

    virtual void update(const float dt);

    virtual void fixedUpdate(const float step);

    virtual void draw(sf::RenderTarget& target);

    sf::View& getView();

    const sf::View& getView() const;

  private:

    virtual void onWindowEvent(const sf::Event& event);

  private:

    std::multimap<uint32_t, std::unique_ptr<Layer>> m_layers;
    sf::View m_view;
  };
}
