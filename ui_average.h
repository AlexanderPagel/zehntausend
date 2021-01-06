// ui_average.h
//
// Declaration of class ui::Average to keep track of the average
// score per round, for each player.

#ifndef UI_AVERAGE_H_INCLUDED
#define UI_AVERAGE_H_INCLUDED 1


#include <vector>

#include "ui_types.h"
#include "ui_subobject.h"


namespace ui
{

template<typename A = double, typename P = Points_t>
class Average : public UiSubobject
{
    using Average_type = A;
    using Points_type = P;
    // A pair (sum, n)
    using AverageStats = std::pair<Points_type, int>;
    // TODO After writing this I would now prefer to have two separate vectors
    //      for sums and counts. Or at least use struct for readability.

    std::vector<AverageStats> stats;

    Points_type const& sumOf(Player_t const&) const;
    Points_type&       sumOf(Player_t const&);
    int const& countOf(Player_t const&) const;
    int&       countOf(Player_t const&);

  public:
    Average_type getAverage(Player_t const&) const;
    void addToAverage(Player_t const&, Points_type const&);

    bool isInit() const;
    void init(Player_t const& playerCount);

    using UiSubobject::UiSubobject; // Requires init() after construction
    explicit Average(Player_t const& playerCount);
};

} // namespace ui


#include "ui_average.inc"


#endif // UI_AVERAGE_H_INCLUDED
