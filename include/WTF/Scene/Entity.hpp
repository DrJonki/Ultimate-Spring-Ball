#pragma once

#include <WTF/Export.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <memory>

namespace sf
{
  class RenderTarget;
}

namespace wtf
{
  class Component;

  class WTF_API Entity
  {
    friend class Layer;

  public:

    Entity(const std::string& name);

    virtual ~Entity() = 0;

    void erase();

    bool isErased() const;

    void setActive(const bool active);

    bool isActive() const;

    const std::string& getName() const;

    template<typename T, typename ... Args>
    T& addComponent(Args&&... args);

    template<typename T>
    T* getComponent();

    template<typename T>
    const T* getComponent() const;

  private:

    void baseUpdate(const float dt);

    void baseFixedUpdate(const float step);

    void baseDraw(sf::RenderTarget& target) const;

    virtual void update(const float dt);

    virtual void fixedUpdate(const float step);

    virtual void draw(sf::RenderTarget& target) const;

  private:

    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
    const std::string m_name;
    bool m_active;
    bool m_deleted;
  };
}
