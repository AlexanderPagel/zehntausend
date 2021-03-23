#include "rl_evaluator.h"

#include <memory>


namespace rl
{

void Evaluator::evaluateTraining(std::shared_ptr<Bot_type> bot)
{
  for (int i = 0; i < parameters.trainingEpisodes; ++i)
  {
    auto finalReturn { bot->performLearningEpisode() };
    // Update runnign average stats
    for (auto const& s : stats)
    {
    }
  }
}

Evaluator::Evaluator(int training, int test)
  : parameters{training, test},
    stats(parameters.steps.size() + 1)
{
  assert(training > 0 && test > 0);
}

Evaluator::Value_type
Evaluator::operator()(std::weak_ptr<Bot_type> bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

} // namespace rl
