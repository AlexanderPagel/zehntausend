// stats.h
//
// TODO Brief explanation.


#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED 1


namespace stats
{

// TODO Move Mean and Stat classes into dedicated stats.h header.
// TODO Put implementation in .inc file
template<typename T, typename M = double,
         typename = std::enable_if_t<std::is_floating_point_v<M>>
        >
class Mean
{
  public:
    using Long_type = uint64_t;
    using Mean_type = M; // M: Floating point type used for computations
    using Value_type = T; // T: Type we want to aggregate and compute the mean over
    // Not sure how much is actually needed, but better not take the 32 bit bet
    static_assert(sizeof(Value_type) >= 8);
    static_assert(sizeof(Long_type) >= 4); // 32 bit should be enough for counting only
    static_assert(sizeof(Mean_type) >= 4); // TODO Makes sense for floats?

  protected:
    Value_type sum{0};
    Long_type n{0};

    Mean_type computeMean() const
    {
      return getN() > 0 ? static_cast<Mean_type>(sum) / static_cast<Mean_type>(n)
                        : 0;
    }

  public:
    Value_type getSum() const { return sum; }
    Long_type getN() const { return n; }
    bool hasMean() const { return getN() > 0; }

    // Easiert to use from outside
    Mean_type operator()() const { computeMean(); }
    Mean& operator+=(Value_type t) { sum += t; ++n; return *this; }
    // Easier to type in derived classes
    auto getMean() const { return this->oeprator()(); }
    auto addToMean(Value_type t) { return this->operator+=(t); }
};

template<typename T, typename M = double>
class Stats : public Mean<T, M>
{
  private:
    using Base_type = Mean<T,M>;

  public:
    using Long_type = typename Base_type::Long_type;
    using Mean_type = typename Base_type::Mean_type;
    using Value_type = typename Base_type::Value_type;
    using Variance_type = Mean_type;
    using StdDeviation_type = Variance_type;

  private:
    Variance_type varianceCumulative {0};

    Mean_type offsetOf(T t) const;

    void updateVariance(T t)
    {
      Mean_type const offset {offsetOf(t)};
      Base_type::addToMean(t);
      varianceCumulative = varianceCumulative + offset * (t - getMean());
    }

  public:
    using Base_type::getN;
    using Base_type::getMean;
    bool hasVariance() const { return getN() > 1; }
    bool gasStdDeviation() const { return hasVariance(); }
    // Sanitize std::abs in case inaccurate float operations produce negative
    Variance_type getVariance() const
    {
      return getN() > 0 ? std::abs(varianceCumulative / static_cast<Variance_type>(getN()))
                        : -1;
    }
    StdDeviation_type getStdDeviation() const { return getN() > 0 ? std::sqrt(getVariance()) : -1; }
    std::pair<Mean_type, Mean_type> operator()() const
    {
      return std::make_pair(getMean(), getStdDeviation());
    }
};

} // namespace stats


#include "stats.inc"


#endif // STATS_H_INCLUDED
