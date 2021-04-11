// ringbuffer.h
//
// Class Ringbuffer provides a linear, cyclic container. Once the last maximum
// size is reached, the next store operation overwrites the first element again
// (and so on). The buffer size (and allocated size) remains constant during
// its lifetime.


#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED 1


#include <cassert>
#include <vector>


template<typename T>
class Ringbuffer
{
    using Value_type = T;

    std::vector<T> buffer;
    int pos {0}; // TODO Using iterator worth it?

    // operator<< would alias overwrite reference otherwise
    bool sizeCheck() const { return size() > 1; }

    int ringIndex(int i) const;
    int offsetBackwards(int i) const;

  public:
    explicit Ringbuffer(int size);
    explicit Ringbuffer(int size, Value_type const& init);

    int size() const;

    Value_type const& front() const;

    // Queries n-th previously inserted value. 0 for last inserted value.
    // Parameter n must be in range [-size() , size()].
    Value_type const& operator[](int) const;
    // Insert new value, returning value that was replaced.
    // TODO use operator+=, else everyone thinks the ring is shifted
    Value_type operator<<(Value_type const&);
    Value_type insert(Value_type const&);
};


#include "ringbuffer.inc"


#endif // RINGBUFFER_H_INCLUDED
