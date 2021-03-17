namespace randomness
{

template<typename T>
T randomReal(T lo, T hi)
{
  static_assert(std::is_floating_point_v<T>);
  assert(lo < hi);
  return std::uniform_real_distribution<T>(lo, hi)(mt);
}

//template<typename D>
//auto useGlobalRandomness(D& d) -> std::function<decltype(d(mt)) ()>
//{
//  return std::bind(d, mt);
//}

} // namespace randomness
