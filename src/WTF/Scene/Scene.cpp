#include <WTF/Scene/Scene.hpp>
#include <WTF/Scene/Layer.hpp>
#include <cassert>

namespace wtf
{
  Scene::Scene()
    : m_layers()
    , m_view()
  {}

  Scene::~Scene()
  {}

  Layer& Scene::addLayer(const uint32_t weight)
  {
    auto uniq = std::make_unique<Layer>();
    auto& ref = *uniq;
    m_layers.emplace(weight, std::move(uniq));

    return ref;
  }

  void Scene::update(const float dt)
  {
    for (auto& i : m_layers) {
      i.second->update(dt);
    }
  }

  void Scene::fixedUpdate(const float step)
  {
    for (auto& i : m_layers) {
      i.second->fixedUpdate(step);
    }
  }

  void Scene::draw(sf::RenderTarget& target)
  {
    for (auto& i : m_layers) {
      i.second->draw(target);
    }
  }

  sf::View & Scene::getView()
  {
    return m_view;
  }

  const sf::View& Scene::getView() const
  {
    return m_view;
  }

  void Scene::onWindowEvent(const sf::Event&)
  {}
}