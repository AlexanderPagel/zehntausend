namespace randomness
{

template<typename T>
T randomReal(T lo, T hi)
{
  static_assert(std::is_floating_point_v<T>);
  assert(lo < hi);
  return std::uniform_real_distribution<T>(lo, hi)(mt);
}

template<typename T>
auto pickUniformRandom(T const& t) -> decltype(t.begin())
{
  std::uniform_int_distribution<decltype(t.size())> ur(0, t.size()-1);
  return t.begin() + ur(mt); // Using global randomness engine
}

//template<typename D>
//auto useGlobalRandomness(D& d) -> std::function<decltype(d(mt)) ()>
//{
//  return std::bind(d, mt);
//}

} // namespace randomness
