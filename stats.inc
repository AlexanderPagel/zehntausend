namespace stats
{

// -----------------------------------------------------------------------------
// Stats
// -----------------------------------------------------------------------------

template<typename T, typename M>
auto Stats<T,M>::offsetFromMean(Value_type v) const -> Mean_type
{
  return getN() != 0 ? static_cast<Mean_type>(v) - getMean()
                     : v;
}

template<typename T, typename M>
auto Stats<T,M>::varFinalize(bool sample) const -> Variance_type
{
  // Divide by N-1 for sample variance
  auto divisor {static_cast<Variance_type>(getN() - int(sample))};

  // Sanitize with std::abs in case inaccurate float operations produce
  // negative.
  // TODO Wikipedia algorithm only computes var for n > 1, rather than n > 0.
  //      Do we need that, too? We would be fine if var is simply 0 even if it
  //      doesnt make sense for just 1 value.
  return std::abs(squareSum / divisor);
}

template<typename T, typename M>
auto Stats<T,M>::stdDeviationFinalize(bool sample) const
  -> StdDeviation_type
{
  return std::sqrt(varFinalize(sample));
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
Stats<T,M>::Stats(Long_type initialCounter)
  : n{initialCounter}
{}

template<typename T, typename M>
auto Stats<T,M>::getMean() const -> Mean_type
{
  return hasMean() ? static_cast<Mean_type>(sum) / static_cast<Mean_type>(n)
                   : 0;
}

template<typename T, typename M>
auto Stats<T,M>::getVariance() const -> Mean_type
{
  return hasVariance() ? varFinalize(false) : -1;
}

template<typename T, typename M>
auto Stats<T,M>::getSampleVariance() const -> Variance_type
{
  return hasVariance() ? varFinalize(true) : -1;
}

template<typename T, typename M>
auto Stats<T,M>::getStdDeviation() const -> StdDeviation_type
{
  return hasStdDeviation() ? stdDeviationFinalize(false) : -1;
}

template<typename T, typename M>
auto Stats<T,M>::getSampleStdDeviation() const -> StdDeviation_type
{
  return hasStdDeviation() ? stdDeviationFinalize(true) : -1;
}

template<typename T, typename M>
auto Stats<T,M>::getRoundaboutConfidence() const -> StdDeviation_type
{
  if (!hasMean() || !hasStdDeviation() || getN() < 1)
  return StdDeviation_type{0};

  auto so = getSampleStdDeviation();
  auto interval {1.96 * so / std::sqrt(static_cast<StdDeviation_type>(getN()))};
  return StdDeviation_type{interval};
}

template<typename T, typename M>
auto Stats<T,M>::operator()() const
  -> StatRecord
{
  return std::make_tuple(
    getN(), getSum(), getMean(), getRoundaboutConfidence(),
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

// -----------------------------------------------------------------------------
// RunningStats
// -----------------------------------------------------------------------------

/*
template<typename T, typename M>
RunningStats<T,M>::RunningStats(Drag_type d, Value_type init)
  : RunningStats(std::make_shared<Buffer_type>(d, Value_type{init}), d)
{}

template<typename T, typename M>
RunningStats<T,M>::RunningStats(std::shared_ptr<Buffer_type> b)
  : RunningStats(b, b->size())
{}

template<typename T, typename M>
RunningStats<T,M>::RunningStats(std::shared_ptr<Buffer_type> b,
                                     Drag_type d)
  : ringBuffer{b}, drag{d}
{
  // Rungbuffer must be large enough
  assert(b->size() >= d);
}

template<typename T, typename M>
auto RunningStats<T,M>::operator+=(Value_type v) -> RunningStats&
{
  // I just noticed the other stats objects don't know about this
  auto old = ringBuffer << v;
}
*/

} // namespace stats
