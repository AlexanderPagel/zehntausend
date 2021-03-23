namespace stats
{

template<typename T, typename M>
auto Mean<T,M>::computeMean() const -> Mean_type
{
  return getN() > 0 ? static_cast<Mean_type>(sum) / static_cast<Mean_type>(n)
                    : 0;
}

template<typename T, typename M>
auto Stats<T,M>::offsetFromMean(T t) const -> Mean_type
{
  return getN() != 0 ? static_cast<Mean_type>(t) - Base_type::getMean()
                     : t;
}

template<typename T, typename M>
auto Stats<T,M>::updateVariance(T t)
{
  // Offset from old mean
  Mean_type const offset { offsetFromMean(t) };
  Base_type::addToMean(t);
  Mean_type const offsetNew { offsetFromMean(t) };
  varianceCumulative += offset * offsetNew;;
}

template<typename T, typename M>
auto Stats<T,M>::operator()() const -> std::pair<Mean_type, Mean_type>
{
  return std::make_pair(getMean(), getStdDeviation());
}

} // namespace stats
