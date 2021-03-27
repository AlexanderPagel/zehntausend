#include "rl_evaluator.h"

#include <memory>
#include <iostream>
#include <iomanip>


namespace rl
{

// FIXME Testing
template<typename T>
void logResult(T const& t)
{
  if (false)
  std::cout << "Result: " << t << std::endl;
}

void
Evaluator::evaluateTraining(Bot_type& bot)
{
  for (int episode = 0; episode < parameters.trainingEpisodes; ++episode)
  {
    // Get new data point
    auto const finalReturn { bot.performLearningEpisode() };
    /*auto const old*/ {buffer << finalReturn;};

    logResult(finalReturn);

    // Update runnign average stats
    for (int i = 0; i < (int)parameters.steps.size(); ++i)
    {
      stats[i] += finalReturn;
      if (episode >= parameters.steps[i])
        stats[i] -= buffer[parameters.steps[i]];
      else
        // Stats have zero-inertia
        stats[i] -= Value_type{0};
    }

    if ((episode + 1) % parameters.steps.front() == 0)
      writeLogEntry(episode + 1);
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

    // FIXME Testing
    logResult(e.getState().getPoints());

    if ((episode+1) % parameters.steps.front() == 0)
      writeLogEntry(episode+1 + parameters.trainingEpisodes);
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
  auto const out = [](Stats_type const& s) -> void
  {
    auto [n, sum, mean, ci, var, o, svar, so] = s();
    cout << fixed << setprecision(0)              << mean << " +- "
         << fixed << setprecision(0) << std::left << ci   << " o "
         << fixed << setprecision(0) << std::left << so   << ",    ";
  };
  for (auto it = stats.crbegin(); it != stats.crend(); ++it)
    out(*it);
  cout << endl;
}

Evaluator::Evaluator(int training, int test)
  : parameters{training, test}, // rest default
    stats{},
//    stats(parameters.steps.size() + 1), // One additional for full evaluation
    // Use buffer one larger so we can treat the largest average in the same
    // loop w/o index overflow.
    buffer(parameters.steps.back() + 1) // Last step size must be largest
{
  assert(training > 0 && test > 0);

  // Create one Stats object for each stepsize
  for (unsigned i = 0; i < parameters.steps.size() ; ++i)
    stats.emplace_back(parameters.steps[i]);
  // Create stats object for final eval
  stats.emplace_back(0);
}

void
Evaluator::operator()(Bot_type& bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

} // namespace rl
