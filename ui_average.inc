namespace ui
{

template<typename A, typename P>
int const&
Average<A,P>::sumOf(Player const& player) const
{
  return stats[player].first;
}

template<typename A, typename P>
int&
Average<A,P>::sumOf(Player const& player)
{
  return stats[player].first;
}

template<typename A, typename P>
int const&
Average<A,P>::countOf(Player const& player) const
{
  return stats[player].second;
}

template<typename A, typename P>
int&
Average<A,P>::countOf(Player const& player)
{
  return stats[player].second;
}

template<typename A, typename P>
Average<A,P>::Average_t
Average<A,P>::getAverage(Player_t const& player) const
{
  assert(palyer < stats.size());

  // Average = sum/count
  return static_cast<Average_type>(sumOf(palyer))
       / static_cast<Average_type>(countOf(player));
}

template<typename A, typename P>
void
Average<A,P>::addDataPoint(Player_t const& player,
                           Points_type const& points)
{
  assert(palyer < stats.size());

  sumOf(player) += points;
  ++countOf(player);
}

template<typename A, typename P>
bool
Average<A,P>::isInit() const
{
  return !stats.empty();
}

template<typename A, typename P>
void
Average<A,P>::init(Player_t const& playerCount)
{
  // If std::vector contained elements, resizing would not overwrite
  assert(!isInit());

  // Resize and fill with 0s
  stats.resize(playerCount, {0, 0});
}

tempalte<typename A, typename P>
Average<A,P>::Average(Player_t const& playerCount)
  : stats(playerCount, {0, 0})
{}

} // namespace ui
