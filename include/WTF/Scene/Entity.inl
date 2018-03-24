template<typename T, typename ...Args>
inline T& Entity::addComponent(Args && ...args)
{
  static_assert(std::is_base_of<Component, T>::value, "Component must be derived from wtf::Component");

  auto uniq = std::make_unique<T>(args...);
  auto ptr = uniq.get();

  m_components[std::type_index(typeid(T))] = std::move(uniq);

  return *ptr;
}

template<typename T>
inline T* Entity::getComponent()
{
  auto itr = m_components.find(std::type_index(typeid(T)));

  if (itr != m_components.end()) {
    return itr->second;
  }

  return nullptr;
}

template<typename T>
inline const T* Entity::getComponent() const
{
  auto& self = const_cast<Entity&>(*this);
  return self.getComponent<T>();
}