template<typename T, typename ...Args>
inline void Engine::pushScene(Args&& ...args)
{
  ms_instance->m_scene = std::make_unique<T>(args...);
}