// rl_evaluator.h
//
// Class "Evaluator" to instantiate and test RL algorithms.


#ifndef RL_EVALUATOR_H_INCLUDED
#define RL_EVALUATOR_H_INCLUDED 1


#include <cmath> // sqrt
#include <memory>
#include <fstream>

#include "sarsa.h" // Bot
#include "stats.h"
#include "ringbuffer.h"


namespace rl
{

using namespace stats;

class Evaluator
{
  public:
    using Bot_type = Sarsa; // TODO Eventually template param?
    using Value_type = Bot_type::Return_t;

    using StatsValue_type = long long;
    using Stats_type = Stats<StatsValue_type>;
    using RunningStats_type = NStats<StatsValue_type>;
    static_assert(std::is_integral_v<StatsValue_type>,
        "Need to adjust Stats<> type parameter");
    static_assert(std::is_convertible_v<Value_type, StatsValue_type>,
        "Returns are processed by stats");

  private:
    static std::string outFileTrain;
    static std::string outFileFinal;

    // TODO Eventually the stats drag should be adjustable by (ctor)
    // Stats with different amount of drag for training
    int trainingEpisodes;
    int evaluationEpisodes;
    RunningStats_type runningStats;
    Stats_type finalStats;
    Ringbuffer<decltype(runningStats.getStats().back().getMean())> meansBuffer;
    // TODO Use as minimum, then train until largest running stats for the first time
    mutable std::ofstream ofsTrain; // TODO have as input to eval functions?
    mutable std::ofstream ofsFinal;

    // Adds the current training mean (largest drag) to meanBuffer, returning
    // true if the new mean is smaller than the mean 'trainingEpisodes'
    // episodes ago.
    bool stopCondition();

  public:

    // Generate training statistics
    void evaluateTraining(Bot_type&);
    // Generate final (stationary) statistics
    void evaluateFull(Bot_type&);

    void writeLogEntry(int) const;
    void writeTrainingLog(int) const;
    void writeEvaluationLog(int) const;

  public:
    Evaluator(int training, int test);

    void operator()(Bot_type&); // Evaluate bot with configurated parameters
    std::string info() const;
};

void defaultEvaluation();

} // namespace rl


#include "rl_evaluator.inc"


#endif // RL_EVALUATOR_H_INCLUDED


// TODO
//  - loading bar for evaluator
