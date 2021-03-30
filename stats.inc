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
auto Stats<T,M>::replaceValue(Value_type v, Value_type w) -> void
{
  addValue(v);
  removeValue(w);
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

template<typename T, typename M>
auto Stats<T,M>::replace(Value_type newVal, Value_type oldVal) -> Stats&
{
  replaceValue(newVal, oldVal);
  return *this;
}

// -----------------------------------------------------------------------------
// RunningStats
// -----------------------------------------------------------------------------

template<typename T, typename M>
RunningStats<T,M>::RunningStats(Drag_type d)
  : drag{d}
{}

template<typename T, typename M>
auto RunningStats<T,M>::operator()(Buffer_type const& buffer) -> RunningStats&
{
  assert(buffer.size() >= drag + 1);
  assert(!(this->getN() > drag)); // Consistently low-passed stats

  if (this->getN() == drag)
    Base_type::replace(buffer.front(), buffer[drag]);
  else // N < drag
    Base_type::operator+=(buffer.front());

  return *this;
}

// -----------------------------------------------------------------------------
// NStats
// -----------------------------------------------------------------------------

template<typename T, typename M>
auto NStats<T,M>::maxLast() const -> bool
{
  assert(stats.size() > 0);

  auto max = stats.front().size();
  for (auto const& s : stats)
    if (s.size() > max) max = s.size();
  return max == stats.back().size();
}

template<typename T, typename M>
auto NStats<T,M>::isOk() const -> bool
{
  return stats.size() > 0 && maxLast();
}

template<typename T, typename M>
NStats<T,M>::NStats()
  : NStats(100000, 10000000, 10000000)
{}

template<typename T, typename M>
template
<
  typename... Types, typename
//  typename/* = std::enable_if_t
//  <
//    std::is_convertible_v
//    <
//      Types...,
//      std::vector<typename NStats<T,M>::Stats_type>
//    >
//  >
//  */
>
NStats<T,M>::NStats(Types&&... args)
  : stats(std::forward<Types...>(args...)),
    // size+1 => insertion does not remove last element immediately
    buffer(stats.back().size() + 1)
{
  assert(isOk());
}

template<typename T, typename M>
auto NStats<T,M>::getStats() const -> std::vector<Stats_type> const&
{
  return stats;
}

template<typename T, typename M>
auto NStats<T,M>::getBuffer() const -> Buffer_type const&
{
  return buffer;
}

template<typename T, typename M>
auto NStats<T,M>::operator+=(Value_type v) -> NStats&
{
  buffer << v;
  auto const updateStat = [&](auto s)
  {
    s(buffer);
  };
  for (auto const& s : stats)
    updateStat(s);
  return *this;
}



} // namespace stats
