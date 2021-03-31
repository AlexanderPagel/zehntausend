namespace stats
{

namespace
{
} // namespace

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
auto RunningStats<T,M>::getDrag() const -> Drag_type
{
  return drag;
}

template<typename T, typename M>
auto RunningStats<T,M>::operator()(Buffer_type const& buffer) -> RunningStats&
{
  assert(buffer.size() >= (int)drag + 1);
  assert(!(this->getN() > drag)); // Consistently low-passed stats

  // If stats are "full", replace oldest value, else only add the new value
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

  auto max = stats.front().getDrag();
  for (auto const& s : stats)
    if (s.getDrag() > max) max = s.getDrag();
  return max == stats.back().getDrag();
}

template<typename T, typename M>
auto NStats<T,M>::isOk() const -> bool
{
  return stats.size() > 0 && maxLast();
}

template<typename T, typename M>
NStats<T,M>::NStats()
  : NStats({100000, 10000000, 10000000})
{}

/*
template<typename T, typename M>
template
<
  typename... Types, typename
//  typename = std::enable_if_t
//  <
//    std::is_convertible_v
//    <
//      Types...,
//      std::vector<typename NStats<T,M>::Stats_type>
//    >
//  >
>
NStats<T,M>::NStats(Types&&... args)
    // FIXME Check how this is working exactly
    // Hand-crafted "emplace_back-like" initializer-list ctor for stats vector
  : stats{(Stats_type(std::forward<Types>(args)), ...)},
    // drag+1 => insertion does not remove last element immediately
    buffer(int(stats.back().getDrag() + 1))
{
  // Largest drag must be last, otherwise buffer is too small
  assert(isOk());

  // FIXME Making sure the pack things are working
  std::cout << "Initializing NStats" << std::endl;
  std::cout << "Number of stats created:" << stats.size() << std::endl;
  std::cout << "buffer length: " << buffer.size() << std::endl;
  std::cout << "individual stats drags:" << std::endl;
  for (auto const& s : stats)
    std::cout << "\t" << s.getDrag() << std::endl;
}
*/

//template<typename T, typename M>
//NStats<T,M>::NStats(int... a)
//  : stats
//{
//}

template<typename T, typename M>
NStats<T,M>::NStats(std::vector<typename Stats_type::Drag_type> drags)
  : stats{},
    buffer(drags.back() + 1)
{
  assert(drags.size() > 0);
  for (auto const& d : drags)
    stats.emplace_back(d);
  // Largest drag must be last, otherwise buffer is too small
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
  buffer << v; // Buffer is 1 larger than needed, so we can ignore return value
  auto const updateStat = [&](auto& s) { s(buffer); };
  std::for_each(std::begin(stats), std::end(stats), updateStat);
  return *this;
}

// -----------------------------------------------------------------------------
// Nonmember functions
// -----------------------------------------------------------------------------

template<typename TT, typename MM>
auto operator<<(std::ostream& os, Stats<TT,MM> const& stats) -> std::ostream&
{
  using namespace std;
  auto const& [n, sum, mean, ci, var, o, svar, so] = stats();
  // Format such that the raw numbers have their own columns
  os <<            fixed << setprecision(2) << mean
     << " (+- " << fixed << setprecision(2) << ci << " )"
     << " o "   << fixed << setprecision(2) << so;
  return os;
}

template<typename T, typename M>
auto operator<<(std::ostream& os, NStats<T,M> const& runningStats)
  -> std::ostream&
{
  assert(runningStats.stats.size() > 0);

  auto const& stats {runningStats.getStats()};

  // Output drag values collectively at beginning of line
  using namespace std;
  os << "[" << stats.front().getDrag();
  for (auto it = std::next(stats.begin()); it != stats.end(); ++it)
    os << "," << it->getDrag();
  os << "]";

  // Individually append each stats object
  for (auto const& s : stats)
    os << " " << s;

  return os;
}

} // namespace stats
