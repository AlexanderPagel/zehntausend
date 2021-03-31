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
    // TODO Eventually the stats drag should be adjustable by (ctor)
    // Stats with different amount of drag for training
    NStats<StatsValue_type> runningStats {{10000, 300000, 10000000}};
    Stats<StatsValue_type> finalStats;
    // TODO Use as minimum, then train until largest running stats for the first time
    int trainingEpisodes;
    int evaluationEpisodes;
    mutable std::ofstream ofsTrain; // TODO have as input to eval functions?
    mutable std::ofstream ofsFinal;

  public:
    static std::string outFileTrain;
    static std::string outFileFinal;

    // Generate training statistics
    void evaluateTraining(Bot_type&);
    // Generate final (stationary) statistics
    void evaluateFull(Bot_type&);

    void writeLogEntry(int) const;
    void writeTrainingLog(int) const;
    void writeEvaluationLog(int) const;

  public:
    Evaluator(int training, int test);

    // Evaluate bot with the configurated parameters
    void operator()(Bot_type&);
};

} // namespace rl


#include "rl_evaluator.inc"


#endif // RL_EVALUATOR_H_INCLUDED


// Questions:
//  1 Evaluate bot during training or after training? Both? Running training?
//    - Must at least reset some time to evaluate full strength of bot + no exploration
//  2 What to evaluate?
//    - Mean return + Std deviation
//    - 20k episode average, 100k episodes average, 400k episodes average, running average of training?
//      -> Includes exploration(!)
//      -> log to file for plotting later

// TODO
//  - loading bar for evaluator
