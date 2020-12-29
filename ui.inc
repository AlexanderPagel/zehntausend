namespace ui
{

template<typename T,
         typename = decltype(UiFactory::set(new T))
        >
UiFactory& UiFactory::operator<<(T* t)
{
  return set(t);
}

} // namespace ui
