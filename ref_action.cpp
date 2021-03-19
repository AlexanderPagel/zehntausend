#include "ref_action.h"

#include "ref_throw.h"


namespace refac
{

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

} // namespace refac
