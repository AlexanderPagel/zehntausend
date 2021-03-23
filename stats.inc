namespace stats
{

template<typename T, typename M>
auto Stats<T,M>::offsetOf(T t) const
-> Mean_type
{
  return getN() != 0 ? static_cast<Mean_type>(t) - Base_type::getMean()
                     : t;
}

//template<typename T, typename M>
//template<typename T, typename M>
//template<typename T, typename M>
//template<typename T, typename M>
//template<typename T, typename M>

} // namespace stats
