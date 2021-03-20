#include "ref_types.h"

#include "enum.h"


namespace refac
{

namespace
{
auto constexpr offsetDigitToDigitType = raw(DigitType::one) - 1;
auto constexpr offsetDigitTypeToDigit = -offsetDigitToDigitType;
}

DigitType
digitToDigitType(Digit_t digit)
{
  // Add offset int -> DigitType
  auto const res = DigitType(digit + offsetDigitToDigitType);

  assert(legit(res));

  return res;
}

Digit_t
digitTypeToDigit(DigitType digit)
{
  assert(legit(digit));

  return raw(digit) + offsetDigitTypeToDigit;
}

} // namespace refac
