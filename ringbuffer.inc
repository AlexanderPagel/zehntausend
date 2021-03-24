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
{}

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
  // TODO Can remove copying if we secretly make buffer
  //      1 larger. But then we need toconsider that when
  //      looking forward (maybe just disallow).
  auto tmp {buffer[pos]};
  buffer[pos] = v;
  pos = ringIndex(pos + 1);
  return tmp;
}
