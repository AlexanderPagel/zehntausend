// stats.h
//
// Implements a mean and variance computation. Contrary to the algorithms
// below, we compute the average exactly by summizing over all values. We use
// these stats for RL algorithm evaluation and returns are typically in range
// [0, 2000].

// References:
//
// Wikipedia [https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance]:
//  # For a new value newValue, compute the new count, new mean, the new M2.
//  # mean accumulates the mean of the entire dataset
//  # M2 aggregates the squared distance from the mean
//  # count aggregates the number of samples seen so far
//  def update(existingAggregate, newValue):
//      (count, mean, M2) = existingAggregate
//      count += 1
//      delta = newValue - mean
//      mean += delta / count
//      delta2 = newValue - mean
//      M2 += delta * delta2
//      return (count, mean, M2)
//
//  # Retrieve the mean, variance and sample variance from an aggregate
//  def finalize(existingAggregate):
//      (count, mean, M2) = existingAggregate
//      if count < 2:
//          return float("nan")
//      else:
//          (mean, variance, sampleVariance) = (mean, M2 / count, M2 / (count - 1))
//          return (mean, variance, sampleVariance)
//
// Maybe implement removal operation later.
// [https://stackoverflow.com/questions/30876298/removing-a-prior-sample-while-using-welfords-method-for-computing-single-pass-v]:
//  Mk = Mk-1 + (xk – Mk-1) / k
//  Sk = Sk-1 + (xk – Mk-1) * (xk – Mk)
//  =>
//  Mk-1 = Mk - (xk - Mk) / (k - 1).
//  Sk-1 = Sk - (xk – Mk-1) * (xk – Mk).
// Looks stable enough to me since the changes look very similar in size to the
// adding terms. Rouding problems should be about same size as for adding case.
// I only intent to use for runnign average anyway, so low noise should not be
// able to accumulate.


#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED 1


#include <cmath>
#include <tuple>


namespace stats
{

// TODO Move Mean and Stat classes into dedicated stats.h header.
// TODO Put implementation in .inc file
template<typename T, typename M = double>
class Stats
{
    static_assert(std::is_floating_point_v<M>);

  public:
    using Long_type = uint64_t;
    using Value_type = T; // T: Type we want to aggregate and compute the mean over
    using Mean_type = M; // M: Floating point type used for computations
    using Variance_type = Mean_type;
    using StdDeviation_type = Variance_type;
    // Not sure how much is actually needed, but better not take the 32 bit bet
    static_assert(sizeof(Value_type) >= 8); // We can use templates to ensure this automatically
    static_assert(sizeof(Long_type) >= 4); // 32 bit should be enough for counting only
    static_assert(sizeof(Mean_type) >= 4); // TODO Makes sense for floats?

  protected:
    Long_type n{0};
    Value_type sum{0};
    Variance_type squareSum { 0 };

    Mean_type offsetFromMean(Value_type v) const;
    Variance_type varFinalize(bool sample) const;
    StdDeviation_type stdDeviationFinalize(bool sample) const;


    // Add *only* to the mean parts 'n' and 'sum'
    void addToMean(Value_type v);
    void removeFromMean(Value_type v);
    // Update mean *and* variance parts
    void addValue(Value_type v);
    void removeValue(Value_type v);

  public:
    bool hasMean() const { return getN() > 0; }
    bool hasVariance() const { return getN() > 1; }
    bool hasStdDeviation() const { return hasVariance(); }

    Long_type getN() const { return n; }
    Value_type getSum() const { return sum; }
    Mean_type getMean() const;
    Variance_type getVariance() const;
    Variance_type getSampleVariance() const;
    StdDeviation_type getStdDeviation() const;
    StdDeviation_type getSampleStdDeviation() const;

    // Get (N, sum, mean, var, o, sample_var, sample_o)
    using StatRecord
      = std::tuple<Long_type, Value_type, Mean_type, Variance_type,
                   StdDeviation_type, Variance_type, StdDeviation_type>;
    StatRecord operator()() const;

    // Easiert to use from outside
    Stats& operator+=(Value_type t);
    Stats& operator-=(Value_type t);
    // Easier to type in derived classes
};

} // namespace stats


#include "stats.inc"


#endif // STATS_H_INCLUDED
