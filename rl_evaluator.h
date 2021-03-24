// rl_evaluator.h
//
// Class "Evaluator" to instantiate and test RL algorithms.


#ifndef RL_EVALUATOR_H_INCLUDED
#define RL_EVALUATOR_H_INCLUDED 1


#include <cmath> // sqrt
#include <memory>

#include "sarsa.h" // Bot
#include "stats.h"
#include "ringbuffer.h"


namespace rl
{

using namespace stats;

class Evaluator
{
  private:

  public:
    using Bot_type = Sarsa; // TODO Eventually template param?
    using Value_type = Bot_type::Return_t;

  private:
    struct Parameters
    {
      int trainingEpisodes;
      int evaluationEpisodes;
      // Step sizes for  runnign averages during training
      // TODO Concrete parameters up for debate.
      std::vector<int> steps { 20000, 100000, 400000 };
    } parameters;
    std::vector<Stats<Value_type>> stats;
    Ringbuffer<Value_type> buffer; // Store values for running average

    // Generate training statistics
    void evaluateTraining(Bot_type&);
    // Generate final (stationary) statistics
    void evaluateFull(Bot_type&);

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
