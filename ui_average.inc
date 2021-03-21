namespace ui
{

template<typename A, typename P>
typename Average<A,P>::Points_type const&
Average<A,P>::sumOf(Player_t const& player) const
{
  return stats[player].first;
}

template<typename A, typename P>
typename Average<A,P>::Points_type&
Average<A,P>::sumOf(Player_t const& player)
{
  return stats[player].first;
}

template<typename A, typename P>
int const&
Average<A,P>::countOf(Player_t const& player) const
{
  return stats[player].second;
}

template<typename A, typename P>
int&
Average<A,P>::countOf(Player_t const& player)
{
  return stats[player].second;
}

template<typename A, typename P>
typename Average<A,P>::Average_type
Average<A,P>::getAverage(Player_t const& player) const
{
  assert(player < (int)stats.size());

  if (countOf(player) == 0)
    return -1;

  // Average = sum/count
  return static_cast<Average_type>(sumOf(player))
       / static_cast<Average_type>(countOf(player));
}

template<typename A, typename P>
void
Average<A,P>::addToAverage(Player_t const& player,
                           Points_type const& points)
{
  assert(player < (int) stats.size());

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

template<typename A, typename P>
Average<A,P>::Average(Ui* ui, Player_t const& playerCount)
  : UiSubobject(ui), stats(playerCount, {0, 0})
{}

} // namespace ui
