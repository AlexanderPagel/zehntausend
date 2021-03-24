#include "rl_evaluator.h"

#include <memory>
#include <iostream>
#include <iomanip>


namespace rl
{

void
Evaluator::evaluateTraining(Bot_type& bot)
{
  for (int episode = 0; episode < parameters.trainingEpisodes; ++episode)
  {
    // Get new data point
    auto const finalReturn { bot.performLearningEpisode() };
    /*auto const old*/ {buffer << finalReturn;};

    // Update runnign average stats
    for (int i = 0; i < (int)parameters.steps.size(); ++i)
    {
      stats[i] += finalReturn;
      if (episode >= parameters.steps[i])
        stats[i] -= buffer[parameters.steps[i]];
    }

    if (episode % parameters.steps.front() == 0)
      writeLogEntry(episode);
  }
}

void
Evaluator::evaluateFull(Bot_type& bot)
{
  // Stats object to be used
  auto& s {stats.back()};

  Bot_type::Environment_t e;
  for (int episode = 0; episode < parameters.evaluationEpisodes; ++episode)
  {
    // Generate new data point
    for (e.restart(); !e.episodeFinished();
         e.takeAction(bot.greedy(e.getState())))
      ; // Empty
    s += e.getState().getPoints(); // TODO Environment::getReturn().

    if (episode % parameters.steps.front() == 0)
      writeLogEntry(episode + parameters.trainingEpisodes);
  }
}

void
Evaluator::writeLogEntry(int i) const
{
  using namespace std;
  // TODO For now we just use stdout. Can re-asses the output method later. The
  //      nubmer of logs should be small compared to the number of training
  //      steps in between.
  cout << i << ":   ";
  auto const out = [](Stats<Value_type> const& s) -> void
  {
    auto [n, sum, mean, var, o, svar, so] = s();
    cout << fixed << setprecision(0)              << mean << " o "
         << fixed << setprecision(0) << std::left << so << ",   ";
  };
  for (auto it = stats.crbegin(); it != stats.crend(); ++it)
    out(*it);
  cout << endl;
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
