#include "ui.h"


namespace ui
{

template<typename T>
bool
Rollback<T>::hasObject() const
{
  return t != nullptr;
}

template<typename T>
void
Rollback<T>::clear()
{
  if (!hasObject())
    return;
  delete t;
  t = nullptr;
}

template<typename T>
void
Rollback<T>::save(T const& game)
{
  clear();
  t = new T(game);
}

template<typename T>
void
Rollback<T>::restore(T& game) const
{
  assert(hasObject());
  game = *t;
}

template<typename T>
Rollback<T>::~Rollback()
{
  clear();
}

}
