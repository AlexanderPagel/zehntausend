namespace refac
{

} // namespace refac

// Provided for RL algorithm that wants to cache lega lactions vector
template<>
struct std::hash<refac::State>
{
  size_t operator()(refac::State const& state) const
  {
    return std::hash<int>()
    (
      state.isTerminal()
      ? (int) -1
      : ( (int) state.getThrown()[refac::DigitType::one] << 0 |
          (int) state.getThrown().getDigitCount(refac::DigitType::two)   <<  3 |
          (int) state.getThrown().getDigitCount(refac::DigitType::three) <<  6 |
          (int) state.getThrown().getDigitCount(refac::DigitType::four)  <<  9 |
          (int) state.getThrown().getDigitCount(refac::DigitType::five)  << 12 |
          (int) state.getThrown().getDigitCount(refac::DigitType::six)   << 15 |
          (int) state.getPoints()                                        << 18
        )
    );
  }
};

