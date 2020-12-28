#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED 1

//class State
//{
//public:
//    unsigned int points;
//    unsigned int dice;
//    bool terminal = false;

//    auto operator++(void) & -> State& {dice = (dice+1)%7; if( !dice ) points += 50; return *this;}
//    auto operator==(State const& src) const & -> bool{ return src.points == points && src.dice == dice; }
    //auto isFirstState(void) const & -> bool { return points == 0 && dice == 0; }
//    auto isInRange() const & -> bool { return points <= 2000 && dice <= 6 && terminal == false; } // first state AFTER all valid states

//    State(unsigned int p = 0, unsigned int d = 0) : points{p}, dice{d}{}
//};

#endif // STATE_HPP_INCLUDED
