#pragma once

#include <WTF/Scene/Entity.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <type_traits>

namespace wtf
{
  template<typename T>
  class Drawable : public Entity, public T
  {
    static_assert(std::is_base_of<sf::Drawable, T>::value, "wtf::Drawable must derive from sf::Drawable");

  public:

    template<typename ... Args>
    Drawable(const std::string& name, Args&&... args);

  private:

    virtual void draw(sf::RenderTarget& target) const override;
  };

  #include <WTF/Scene/Drawable.inl>
}
