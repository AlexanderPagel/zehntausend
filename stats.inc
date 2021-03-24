namespace stats
{

template<typename T, typename M>
auto Stats<T,M>::offsetFromMean(T t) const -> Mean_type
{
  return getN() != 0 ? static_cast<Mean_type>(t) - Base_type::getMean()
                     : t;
}

template<typename T, typename M>
auto Stats<T,M>::varFinalize(bool sample) -> Variance_type
{
  // TODO Needed?
  assert(squareSum / static_cast<Variance_type>(getN()) >= 0);

  // Divide by N-1 for sample variance
  auto divisor {static_cast<Variance_type>(getN() - int(sample))};

  // TODO Need this sanitization?
  // Sanitize with std::abs in case inaccurate float operations produce
  // negative.
  // TODO Wikipedia algorithm only computes var for n > 1, rather than n > 0.
  //      Do we need that, too? We would be fine if var is simply 0 even if it
  //      doesnt make sense for just 1 value.
  return getN() > 0 ? std::abs(squareSum / divisor)
                    : -1;
}

template<typename T, typename M>
auto Stats<T,M>::stdDeviationFinalize(bool sample) const -> StdDeviation_type
{
  return getN() > 0 ? std::sqrt(varFinalize(sample));
}

template<typename T, typename M>
auto Stats<T,M>::addToMean(Value_type t)
{
  ++n;
  sum += t;
}

template<typename T, typename M>
auto Stats<T,M>::updateVarianceAndMean(T t)
{
  // Offset from old mean
  Mean_type const offset { offsetFromMean(t) };
  Base_type::addToMean(t);
  Mean_type const offsetNew { offsetFromMean(t) };
  squareSum += offset * offsetNew;;
}

template<typename T, typename M>
auto Stats<T,M>::getMean() const -> Mean_type
{
  return getN() > 0 ? static_cast<Mean_type>(sum) / static_cast<Mean_type>(n)
                    : 0;
}

template<typename T, typename M>
auto Stats<T,M>::getVariance() const -> Mean_type
{
  return varFinalize(false);
}

template<typename T, typename M>
auto Stats<T,M>::getSampleVariance() const
{
  return varFinalize(true);
}

template<typename T, typename M>
auto Stats<T,M>::getStdDeviation() const -> StdDeviation_type
{
  stdDeviationFinalize(false);
}

template<typename T, typename M>
auto Stats<T,M>::getSampleStdDeviation() const -> StdDeviation_type
{
  stdDeviationFinalize(true);
}

template<typename T, typename M>
auto Stats<T,M>::operator()() const
  -> std::pair<Mean_type, Mean_type>
{
  return std::make_pair(
    getN(), getSum(), getMean(),
    getVariance(), getStdDeviation(),
    getSampleVariance(), getSampleStdDeviation()
    );
}

template<typename T, typename M>
auto Stats<T,M>::operator+=(Value_type t) -> Stats&
{
  updateVarianceAndMean(t);
  return *this;
}

} // namespace stats
