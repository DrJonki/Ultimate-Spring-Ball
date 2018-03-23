#include <WTF/Scene/Layer.hpp>
#include <WTF/Scene/Entity.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace wtf
{
  Layer::Layer()
    : m_entities    ()
    , m_view        (nullptr)
    , m_defaultView ()
    , m_active      (true)
  {}

  Layer::~Layer()
  {}

  void Layer::update(const float dt)
  {
    if (!isActive())
      return;

    for (auto itr = m_entities.begin(); itr != m_entities.end(); /**/) {
      if (itr->second->isErased()) {
        itr = m_entities.erase(itr);
        continue;
      }

      itr->second->baseUpdate(dt);
      ++itr;
    }
  }

  void Layer::draw(sf::RenderTarget& target)
  {
    if (!isActive())
      return;

    for (auto& i : m_entities) {
      target.setView(getView());
      i.second->baseDraw(target);
    }
  }

  void Layer::setView(const sf::View* const view)
  {
    m_view = view;
  }

  const sf::View& Layer::getView() const
  {
    return m_view ? *m_view : m_defaultView;
  }

  void Layer::setActive(const bool active)
  {
    m_active = active;
  }

  bool Layer::isActive() const
  {
    return m_active;
  }
}
