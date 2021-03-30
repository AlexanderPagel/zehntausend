#include "rl_evaluator.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>


namespace rl
{

// FIXME Testing
template<typename T>
void logResult(T const& t)
{
  if (false)
  std::cout << "Result: " << t << std::endl;
}

std::string
Evaluator::outFile
{
  "stats.dat"
};

void
Evaluator::evaluateTraining(Bot_type& bot)
{
  for (int episode = 0; episode < trainingEpisodes; ++episode)
  {
    // Get new data point
    auto const finalReturn { bot.performLearningEpisode() };
    runningStats += finalReturn;

    // FIXME testing
    logResult(finalReturn);

    if ((episode + 1) % runningStats.getStats().front().getDrag() == 0)
      writeLogEntry(episode + 1);
  }
}

void
Evaluator::evaluateFull(Bot_type& bot)
{
  Bot_type::Environment_t e;
  for (int episode = 0; episode < evaluationEpisodes; ++episode)
  {
    // Generate new data point
    for (e.restart(); !e.episodeFinished();
         e.takeAction(bot.greedy(e.getState())))
      ; // Empty
    finalStats += e.getState().getPoints(); // TODO Environment::getReturn().

    // FIXME Testing
    logResult(e.getState().getPoints());

    if ((episode + 1) % runningStats.getStats().front().getDrag() == 0)
      writeLogEntry(episode+1 + trainingEpisodes);
  }
  writeLogEntry(trainingEpisodes);
}

void
Evaluator::writeLogEntry(int i) const
{
  // TODO For now we just use stdout. Can re-asses the output method later. The
  //      nubmer of logs should be small compared to the number of training
  //      steps in between.
  ofs << i << ":   ";
//  auto const out = [](Stats_type const& s) -> void
//  {
//    auto [n, sum, mean, ci, var, o, svar, so] = s();
//    cout << fixed << setprecision(2)              << mean << " +- "
//         << fixed << setprecision(2) << std::left << ci   << " o "
//         << fixed << setprecision(2) << std::left << so   << ",    ";
//  };
//  out(finalStats);
  ofs << finalStats << "\t\t";
  auto const& stats = runningStats.getStats();
  std::for_each(std::crbegin(stats), std::crend(stats),
      [&](auto const& s){ ofs << s << "\t\t"; });
  ofs << std::endl;
}

void
Evaluator::writeTrainingLog(int i) const
{
  std::cout << std::setw(10) << std::left << i << ":\t" << runningStats <<
    std::endl;
}

void
Evaluator::writeEvaluationLog(int i) const
{
  std::cout << std::setw(10) << std::left << i << ":\t" << finalStats <<
    std::endl;
}

Evaluator::Evaluator(int training, int test)
  : trainingEpisodes{training},
    evaluationEpisodes{test},
    ofs(outFile, std::ios::out | std::ios::trunc)
{
  assert(training > 0 && test > 0);

  if (!ofs.is_open())
    throw std::runtime_error("Could not open outfile in Evaluator()");
  std::cerr << "Opened file \"" << outFile << "\" for output." << std::endl;
}

void
Evaluator::operator()(Bot_type& bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

} // namespace rl
