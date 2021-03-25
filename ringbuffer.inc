template<typename T>
auto Ringbuffer<T>::ringIndex(int i) const -> int
{
  // i must be in (-size, 2*size - 1)
  if (i < 0) i += size();
  else if (i >= size()) i -= size();

  // Result is in [0, size) = [0, size-1] U [0, size-1]
  return i;
}

template<typename T>
auto Ringbuffer<T>::offsetBackwards(int i) const -> int
{
  assert(i < size());
  assert(i > -size());
  return ringIndex(pos - i);
}

template<typename T>
Ringbuffer<T>::Ringbuffer(int len)
  : buffer(len)
{
  assert(len > 1); // Some member won't work otherwise
}

template<typename T>
auto Ringbuffer<T>::size() const -> int
{
  return static_cast<int>(buffer.size());
}

template<typename T>
auto Ringbuffer<T>::operator[](int i) const -> Value_type const&
{
  return buffer[offsetBackwards(i)];
}

template<typename T>
auto Ringbuffer<T>::operator<<(Value_type const& v) -> Value_type
{
  auto const& tmp {buffer[pos]};
  pos = ringIndex(pos + 1);
  buffer[pos] = v;
  return tmp;

  // TODO Is it UB returning buffer[0] if it was never initialized?
}
