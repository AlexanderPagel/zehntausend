#include "rl_evaluator.h"

#include <memory>


namespace rl
{

void
Evaluator::evaluateTraining(Bot_type& bot)
{
  for (int i = 0; i < parameters.trainingEpisodes; ++i)
  {
    // Get new data point
    auto const finalReturn { bot.performLearningEpisode() };
    /*auto const old*/ {buffer << finalReturn;};

    // Update runnign average stats
    for (int i = 0; i < (int)parameters.steps.size(); ++i)
    {
      stats[i] += finalReturn;
      if (i >= parameters.steps[i])
        stats[i] -= buffer[parameters.steps[i]];
    }
  }
}

void
Evaluator::evaluateFull(Bot_type& bot)
{
  // Stats object to be used
  auto& s {stats.back()};

  Bot_type::Environment_t e;
  for (int i = 0; i < parameters.evaluationEpisodes; ++i)
  {
    // Generate new data point
    for (e.restart(); !e.episodeFinished();
         e.takeAction(bot.greedy(e.getState())))
      ; // Empty
    s += e.getState().getPoints(); // TODO Environment::getReturn().
  }
}

Evaluator::Evaluator(int training, int test)
  : parameters{training, test},
    stats(parameters.steps.size() + 1), // One additional for full evaluation
    // Use buffer one larger so we can treat the largest average in the same
    // loop w/o index overflow.
    buffer(parameters.steps.back() + 1) // Last step size must be largest
{
  assert(training > 0 && test > 0);
}

void
Evaluator::operator()(Bot_type& bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

} // namespace rl
