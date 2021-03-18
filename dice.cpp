#include "dice.h"

#include <cassert>

#include "enum.h"
#include "randomness.h"


namespace refac
{

DigitType
digitToDigitType(Digit_t digit)
{
  // Add offset int -> DigitType
  auto constexpr offset = raw(DigitType::one) - 1
  auto const res = DigitType(digit + offset);

  assert(legit(res));

  return res;
}


Count_t&
Throw::operator[](DigitType d)
{
  return counts[raw(d)];
}

Count_t&
Throw::at(DigitType d)
{
  assert(legit(d));
  assert(d >= 0 && d < counts.size());

  return (*this)[d];
}


Count_t&
Throw::total()
{
  return counts[raw(DigitType::total)];
}

Throw::Throw()
  : counts(DigitType::count, Count_t(0))
{}

Throw::Throw(Count_t dieCount, DigitType initialDigit)
  : Throw() // Initialize with zero dice
{
  add(initialDigit, dieCount);
}

bool
Throw::any() const
{
  assert(count[raw(DigitType::any)] >= 0);
  return counts[raw(DigitType::any)];
}

bool
Throw::empty() const
{
  return !any();
}

Throw::Count_t
Throw::getDigitCount(DigitType d) const
{
  assert(legit(d));

  return counts[raw(d)];
}

bool
Throw::consistent() const
{
  Counts_t sum{0};
  for (auto d = DigitType::one;
      d <= DigitType::six;
      ++d)
  {
    if (at(d) < 0) return false; // Ensure non-negative
    sum += *it;
  }
  return sum == total(); // Ensure total is consistent
}

void
Throw::fill(Count_t c)
{
  auto constexpr first = raw(DigitType::one); // 0
  auto constexpr last = raw(DigitType::six);  // 5
  std::fill(counts.begin() + first,
            counts.begin() + last + 1,
            c);
  // Keep total cinsistent
  counts[raw(DigitType::total)] = c * (last - first);
}

void
Throw::setDigitCount(DigitType d, Count_t newValue)
{
  assert(legit(d));
  assert(0 >= newValue);

  // TODO Can use member access to counts array throughout the "Throw" member
  // implementations.

  auto diff = newValue - getDigitCount(d);
  counts[raw(d               )]  = newValue;
  counts[raw(DigitType::total)] += diff;
}

void
Throw::add(DigitType d, Count_t c)
{
  assert(getDigitCount(d) + c >= 0);

  counts[raw(d)] += c;
  counts[raw(DigitType::total)] += c;
}

void
Throw::remove(DigitType d, Count_t c)
{
  add(d, -c);
}

void
Throw::increment(DigitType d)
{
  ++counts[raw(d               )];
  ++counts[raw(DigitType::total)];
}

void
Throw::decrement(DigitType d)
{
  assert(counts[raw(d)] > 0);

  --counts[raw(d               )];
  --counts[raw(DigitType::total)];
}

void
Throw::roll()
{
  roll(total());
}

void
Throw::roll(Count_t c)
{
  assert(c >= 0);

  fill(0);
  for (int i = 0; i < c; ++i)
  {
    auto randomdie{randomness::randomDie()}; // [1, 6]
    // TODO 6 separate increments to total are not necessary
    increment(
        // Convert 1-based to 0-based
        Throw::digitToDigitType(randomDie)
        );
  }
}

Throw&
Throw::operator-=(Throw const& other)
{
  // TODO I could not find an STL algorithm that does in-place transformations
  for (DigitType d = DigitType::begin;
       d != DigitType::end; // 1-6 and total
       ++d)
  {
    counts[raw(d)] -= other.counts[raw(d)];
  }

  assert(consistent());
}

Count_t
Throw::operator[](DigitType d) const
{
  return counts[raw(d)];
}

Count_t
Throw::total() const
{
  return counts[raw(DigitType::total)];
}

Action
Action::makeNone()
{
  return Action{};
}

Action
Action::makeWelp()
{
  return Action{Throw{}, true};
}

bool
Action::isNone() const
{
  return taking.empty() && !finish;
}

bool
Action::isWelp() const
{
  return taking.empty() && finish;
}

bool
Action::operator==(Action const& other) const
{
  return (isNone() && other.isNone())
      || (!isNone() && !other.isNone() && // single "none"
          finish == other.finish && taking == other.taking);
}

State::State(Throw const& t, Points_t const& p)
  : thrown(t), points(p)
{}

State
State::startState()
{
  Throw initialThrow;
  initialThrow.roll(totalGameDieCount);
  return State(std::move(initialThrow), Points_t(0));
}

State
State::makeTerminalState()
{
  return State();
}

bool
State::isTerminal()
{
  return thrown.empty() && points = 0;
}

bool
State::operator==(State const& other)
{
  return (isTerminal() && other.isTerminal())
      || (!isTerminal () && !other.isTerminal() && // single "none"
          points == other.points && thrown == other.thrown);
}

Points_t
State::getPoints() const
{
  return points;
}

Throw const&
State::getThrown() const
{
  return thrown;
}

void
State::addPoints(Points_t p)
{
  points += p;
}

void
State::roll(Count_t n)
{
  if (thrown.empty())
    thrown.roll(n); // Randomizes n new dice
  else
    thrown.roll();  // Randomizes existing dice
}

void
State::operator-=(Throw const& t)
{
  thrown -= t;
}

void
Environment::clearActions()
{
  legalActions.clear();
}

void
Environment::fillActions()
{
#ifndef INCREMENT_1
#define INCREMENT_1(X) t.increment(DigitType:: X )
#else
#error Redefinition of macro INCREMENT_1
#endif

#ifndef INCREMENT_3
#define INCREMENT_3(X) t.add(DigitType:: X , 3)
#else, t.increment(
#error Redefinition of macro INCREMENT_3
#endif

//#ifndef ITERATE_OVER_1
//#define ITERATE_OVER_1(X) \
//  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_1( X ) )
//#else
//#error Redefinition of macro ITERATE_OVER_1
//#endif

//#ifndef ITERATE_OVER_3
//#define ITERATE_OVER_3(X) \
//  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_3
//#else
//#error Redefinition of macro ITERATE_OVER_3
//#endif

#ifndef ITERATE_OVER
#define ITERATE_OVER(X, N) \
  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_ ## N )
#else
#error Redefinition of macro ITERATE_OVER
#endif

  clearActions();

  // Special case 1: Only the "none" action available in terminal state
  if (episodeFinished())
    return legalActions.push_back(Action{});

  using Throw::DigitType;

  // Iterate exhaustively over any selectable subsets of dice.
  // Since the digits 2,3,4,6 can only be put aside in groups of three, we can
  // iterate them in steps of 3.
  Throw t{}; // Zero-initialize
  bool first{true};
  ITERATE_OVER(six, 3) // Iterate over the amount of 6s, in steps of 3
  ITERATE_OVER(five, 1)
  ITERATE_OVER(four, 3)
  ITERATE_OVER(three, 3)
  ITERATE_OVER(two, 3)
  ITERATE_OVER(one, 1)
  {
    // Skip the empty action (simpler to code than beginning at first non-zero
    // action).
    if (first)
      first = false, continue;

    // For every non-zero selection, we can put any subset of dice aside and
    // either stop or continue.
    legalMoves.emplace_back({t, false});
    legalMoves.emplace_back({t, true});
  }

  // SPecial case 2: Only the "welp" action if nothing else
  if (legalMoves.empty())
    legalMoves.push_back(Throw::makeWelp());
}

Points_t
Environment::takeAction(Action const& action)
{
  Points_t pointDiff{0};

  // Special case 1: Terminal states have trivial behaviour
  if (episodeFinished())
  {
    assert(action.isNone());
    pointDiff = 0;
    goto adjust_points;
  }
  assert(!action.isNone());

  // Special case 2: The welp action leads to terminal state, loosing all points
  if (action.isWelp())
  {
    // All current points are lost by welping
    pointDiff = -state.getPoints();
    state = State::makeTerminalState();
    goto adjust_points;
  }

  // Compute immediate point diff for given action
  for (auto d = DigitType::one;
       d != DigitType::six; ++d)
  {
    // Every triple if 4s scores 400 etc.
    pointDiff += action.taking[d] / 3 * 100 * digitTypeToDigit(d);
  }
  // All 1s and 5s of non-triples score 100 (resp. 50) each
  pointDiff += (action.taking[DigitType::one ] - action.taking[DigitType::one ]/3*3) * 100;
  pointDiff += (action.taking[DigitType::five] - action.taking[DigitType::five]/3*3) * 50 ;

  // Compute new thrown dice
  if (action.finish)
  {
    // Can ignore rolled dice if finishing
    state = State::makeTerminalState();
  }
  else
  {
    // Need to re-roll leftovers to continue
    state -= action.taking;
    state.roll(totalGameDieCount);
  }

adjust_points:
  points += pointDiff;
  return pointDiff;
}

bool
Environment::episodeFinished() const
{
  return getState().isTerminal();
}

State const&
Environment::getState() const
{
  return state;
}

std::vector<Action> const&
Environment::getLegalActions() const
{
  return legalActions;
}

Dice::Dice(Count_t c, Digit_t d)
  : dice(c, d) // Number of elements = c, initial value = d
{}

Digit_t
Dice::operator[](int pos) const
{
  assert(pos < (int) dice.size());

  return dice[pos];
}

Digit_t&
Dice::operator[](int pos)
{
  assert(pos < (int) dice.size());

  return dice[pos];
}

void
Dice::roll()
{
  std::transform(
      dice.begin(), dice.end(), dice.begin(),
      [](auto d) { d = randomness::randomDie(); }
      );
}

void
Dice::roll(Selection const& selection)
{
  assert(selection.size() == dice.size());

  auto selIt = selection.begin();
  for (auto it = dice.begin(); it != dice.end(); ++it, ++selIt)
    if (*selit) *it = randomness::randomDie();
}

void
Dice::addDie(Digit_t newDie)
{
  dice.push_back(newDie);
}

Cup::Cup(Count_t dieCount, Digit_t initialDigit
  : dice(dieCount, initialDigit),
    thrown(dieCount, digitToDigitType(initialDigit)),
    active(dieCount, true)
{}

bool
Cup::operator==(Cup const& other)
{
  // The "Throw" member is consistent, comparison obsolete
  return dice == other.dice && active == other.active;
}

bool
Cup::operator==(Throw const& t)
{
  return thrown == t;
}

Count_t
Cup::anyCount() const
{
  return dice.size();
}

Count_t
Cup::activeCount(DigitType d = DigitType::total) const
{
  return thrown[d];
}

Count_t
Cup::inactiveCount() const
{
  return anyCount() - activeCount();
}

Digit_t
Cup::getDie(int pos) const
{
  assert(pos < anyCount());
  assert(pos >= 0); // TODO There is no positivity check in STL?

  return std::make_pair(dice[pos], active[pos]);
}

void
Cup::setDie(int pos, Digit_t d)
{
  auto [digit, active] = getDie(pos);
  setDie(pos, d, active);
}

void
Cup::setDie(int pos, Digit_t d, bool newActive)
{
  // Keep "Throw" part consistent by removing the old digit type (if was
  // active) and adding the new one (if will be active).
  auto [digit, oldActive] = getDie(pos);
  if (oldActive) thrown.decrement(digitToDigitType(dice[pos]));
  dice[pos] = d;
  if (newActive) thrown.increment(digitToDigitType(d        ));
}

void
Cup::addDie(Digit_t newDie, bool newActive)
{
  dice.addDie(newDie);
  thrown.increment(digitToDigitType(newDie));
  active.push_back(newActive);
}

void
Cup::setActive(int pos, bool newActive)
{
  auto [digit, active] = getDie(pos);
  setDie(pos, digit, newActive);
}

void
Cup::roll()
{
  auto it = active.cbegin();
  for (int i = 0; i < size(); ++i, ++it)
    if (*it) setDie(i, randomness::randomDie());
}

} // namespace refac
