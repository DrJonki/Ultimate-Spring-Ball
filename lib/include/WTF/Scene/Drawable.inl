template<typename T>
template<typename ...Args>
inline Drawable<T>::Drawable(const std::string& name, Args&&... args)
  : T(...args)
{}

template<typename T>
inline void Drawable<T>::draw(sf::RenderTarget& target) const
{
  target.draw(*this);
}