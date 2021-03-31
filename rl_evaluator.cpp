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
Evaluator::outFileTrain
{
  "stats_train.dat"
};

std::string
Evaluator::outFileFinal
{
  "stats_final.dat"
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
      writeTrainingLog(episode+1);
  }
  // Always log last available data point irrespective of division rule above
  writeTrainingLog(trainingEpisodes);
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
      writeEvaluationLog(episode + 1);
  }
  // Always log last available data point irrespective of division rule above
  writeEvaluationLog(evaluationEpisodes);
  // Dirty hack so that evaluation graph is at least as long as training graph
  writeEvaluationLog(trainingEpisodes);
}

void
Evaluator::writeTrainingLog(int i) const
{
  ofsTrain << i << ":\t" << runningStats << std::endl;
}

void
Evaluator::writeEvaluationLog(int i) const
{
  ofsFinal << i << ":\t" << finalStats << std::endl;
}

Evaluator::Evaluator(int training, int test)
  : trainingEpisodes{training},
    evaluationEpisodes{test},
    // FIXME test predetermined grads
    runningStats
    {
      unsigned(0.001 * training),
      unsigned(0.5 * training),
      unsigned(1),
       // Create 1000 data points
//       static_cast<unsigned>(0.001 * training),
       // Geometric mean
//       static_cast<unsigned>(sqrt(0.001 * training * 0.2 * training)),
       // last fivth of training is used to determine stop condition (later)
//       static_cast<unsigned>(       training),
    },
    ofsTrain(outFileTrain, std::ios::out | std::ios::trunc),
    ofsFinal(outFileFinal, std::ios::out | std::ios::trunc)
{
  assert(training > 0 && test > 0);

  if (!ofsTrain.is_open())
    throw std::runtime_error("Could not open training outfile in Evaluator()");
  if (!ofsFinal.is_open())
    throw std::runtime_error("Could not open final outfile in Evaluator()");
  std::cerr << "Opened file \"" << outFileTrain << "\" for output." << std::endl;
  std::cerr << "Opened file \"" << outFileFinal << "\" for output." << std::endl;
}

void
Evaluator::operator()(Bot_type& bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

} // namespace rl
