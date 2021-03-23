// rl_evaluator.h
//
// Class "Evaluator" to instantiate and test RL algorithms.


#ifndef RL_EVALUATOR_H_INCLUDED
#define RL_EVALUATOR_H_INCLUDED 1


#include <cmath> // sqrt
#include <memory>

#include "sarsa.h" // Bot
#include "stats.h"


namespace rl
{

using namespace stats;

class Evaluator
{
  private:

  public:
    using Bot_type = Sarsa; // TODO Eventually template param?
    using Value_type = double;

  private:
    struct Parameters
    {
      int trainingEpisodes;
      int evaluationEpisodes;
      // Step sizes for  runnign averages during training
      std::vector<int> steps { 20000, 100000, 400000 };
    } parameters;
    std::vector<Stats<double>> stats;

    void evaluateTraining(std::shared_ptr<Bot_type>);
    void evaluateFull(std::shared_ptr<Bot_type>);

  public:
    Evaluator(int training, int test);

    // Evaluate bot with the configurated parameters
    Value_type operator()(std::shared_ptr<Bot_type>);
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
