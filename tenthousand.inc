namespace from_rl_bases
{
} // namesapce from_rl_bases

namespace std
{

template<> struct hash<from_rl_bases::TenKState>
{
    size_t operator()(from_rl_bases::TenKState const& x) const
    {
        return std::hash<unsigned int>()
        (
            x.isTerminal()
            ? (1 << (sizeof(unsigned int)*8 -1))
            : ( x.thrown()[0] << 0  |
                x.thrown()[1] << 3  | // max size is 8 ^= 3 bit
                x.thrown()[2] << 6  |
                x.thrown()[3] << 9  |
                x.thrown()[5] << 12 |
                x.thrown()[4] << 15 |
                x.thrown()[6] << 18 |
                x.points()    << 21)
         );
    }
};

template <> struct hash<tenthousand_states::Afterstate>
{
    size_t operator()(tenthousand_states::Afterstate const& x) const
    {
        return std::hash<unsigned int>()
        (
            x.fromTerminal() // 0/0
            ? (1 << (sizeof(unsigned int)*8 -1))
            : ( x.diceLeft()     <<   0 |
                x.points()       <<   3 |
                x.pointsBefore() <<  15) // reserving 12 bit for < 4096 points
        );
    }
};

} // namespace std

