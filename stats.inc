namespace stats
{

template<typename T, typename M>
auto Stats<T,M>::offsetFromMean(Value_type v) const -> Mean_type
{
  return getN() != 0 ? static_cast<Mean_type>(v) - getMean()
                     : v;
}

template<typename T, typename M>
auto Stats<T,M>::varFinalize(bool sample) const -> Variance_type
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
auto Stats<T,M>::stdDeviationFinalize(bool sample) const
  -> StdDeviation_type
{
  return getN() > 0 ? std::sqrt(varFinalize(sample))
                    : -1;
}

template<typename T, typename M>
auto Stats<T,M>::addToMean(Value_type v) -> void
{
  ++n;
  sum += v;
}

template<typename T, typename M>
auto Stats<T,M>::removeFromMean(Value_type v) -> void
{
  assert(getN() > 0);
  --n;
  sum -= v;
}

template<typename T, typename M>
auto Stats<T,M>::addValue(Value_type v) -> void
{
  Mean_type const offset { offsetFromMean(v) };
  addToMean(v);
  Mean_type const offsetNew { offsetFromMean(v) };
  squareSum += offset * offsetNew;;
}

template<typename T, typename M>
auto Stats<T,M>::removeValue(Value_type v) -> void
{
  auto oldDiff {offsetFromMean(v)};
  removeFromMean(v);
  auto newDiff {offsetFromMean(v)};
  squareSum -= oldDiff * newDiff;
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
auto Stats<T,M>::getSampleVariance() const -> Variance_type
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
  -> StatRecord
{
  return std::make_pair(
    getN(), getSum(), getMean(),
    getVariance(), getStdDeviation(),
    getSampleVariance(), getSampleStdDeviation()
    );
}

template<typename T, typename M>
auto Stats<T,M>::operator+=(Value_type v) -> Stats&
{
  addValue(v);
  return *this;
}

template<typename T, typename M>
auto Stats<T,M>::operator-=(Value_type v) -> Stats&
{
  removeValue(v);
  return *this;
}

} // namespace stats
