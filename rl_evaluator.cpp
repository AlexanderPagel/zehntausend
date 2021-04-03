#include "rl_evaluator.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>

#include "utils.h"


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
  "eval/0_training.dat"
};

std::string
Evaluator::outFileFinal
{
  "eval/0_final.dat"
};

// Must make sure to call for updates after each relevant change to training
// stats.
// TODO Fix the above: Create proxy member that is called to update both the
//      stats and the stop condition. Use meanBuffer that is 1 larger than
//      needed to allow separate tto allow separate test and update.
bool
Evaluator::stopCondition()
{
  auto const& slowStats {runningStats.getStats().back()};
  // If our trainign resolution is not reached we ignore the mean. This stops
  // immediate abortion after $trainingEpisodes episodes if the first episode
  // was a high outlier (making the average after just the first episode very
  // large). Alternatively, slats could use zero-inertia but that look ugly on
  // graph.
  auto const newMean
  { slowStats.isFull() ?          slowStats.getMean()
                       : decltype(slowStats.getMean()) {0}};
  auto const oldMean {meansBuffer << newMean};
  return oldMean > newMean;
}

void
Evaluator::evaluateTraining(Bot_type& bot)
{
  int episode {0};
  auto const runEpisode = [&]()
  {
      // Get new data point
      auto const finalReturn { bot.performLearningEpisode() };
      runningStats += finalReturn; // TODO rename to "trainingStats"

      // 1000 points per $trainingEpisodes episodes
      if ((episode + 1) % runningStats.getStats().front().getDrag() == 0)
        writeTrainingLog(episode+1);

      ++episode;
  };

  // TODO Test this idea: Use draggin stats and then compare after the first
  //      'trainingEpisodes' learning episodes already. In the current way, we
  //      need to train at least 2x the amount requested by the user as stop
  //      condition.
  for (; episode < trainingEpisodes; stopCondition())
    runEpisode(); // Keep updating stop condition
  do runEpisode(); while (!stopCondition());

  // Always log last available data point irrespective of division rule above
  writeTrainingLog(episode);
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

  // TODO Dirty hack does not work since using dynamic abort
  // Dirty hack so that evaluation graph is at least as long as training graph
  // TODO Eventually maybe simplify away 'evaluationEpisodes' and just use the
  //      training episode count. Is natural since that is about our accuray
  //      anyway. Train like that will be (estimated) 5x longer than
  //      evaluation, which is also reasonable.
  writeEvaluationLog(trainingEpisodes);
}

void
Evaluator::writeTrainingLog(int i) const
{
  // TODO old version without centering drag intervals:
//  ofsTrain << i << ":\t" << runningStats << std::endl;

  // True counter
  ofsTrain << i << "|\t";

  // Print each internal drag-stats object with an individual offset, centering
  // the stats around the datapoint in the middle of the drag interval.
  auto const& stats {runningStats.getStats()};
  for (auto it {std::crbegin(stats)}; it != std::crend(stats); ++it)
  {
    auto const& s {*it};
//    ofsTrain << " " << i - s.getDrag()/2 << ": " << s;
    ofsTrain << " " << i - s.getOffset() << ": " << s;
  }
  ofsTrain << std::endl;
}

void
Evaluator::writeEvaluationLog(int i) const
{
  ofsFinal << i << "|\t" << finalStats << std::endl;
}

Evaluator::Evaluator(int training, int test)
  : trainingEpisodes{training},
    evaluationEpisodes{test},
    // FIXME test predetermined grads
    runningStats
    {
      int(0.001 * training),
      int(training),
      2, // two in-betweeners
      false, // No inertia
    },
    meansBuffer(training), // No initial content
    ofsTrain(outFileTrain, std::ios::out | std::ios::trunc),
    ofsFinal(outFileFinal, std::ios::out | std::ios::trunc)
{
  assert(training > 0 && test > 0);

  if (!ofsTrain.is_open())
    throw std::runtime_error(
        std::string() + "Could not open outfile \"" + outFileTrain + "\"");
  if (!ofsFinal.is_open())
    throw std::runtime_error(
        std::string() + "Could not open outfile \"" + outFileFinal + "\"");
  std::cerr << "Opened file \"" << outFileTrain << "\" for output." << std::endl;
  std::cerr << "Opened file \"" << outFileFinal << "\" for output." << std::endl;
}

void
Evaluator::operator()(Bot_type& bot)
{
  evaluateTraining(bot);
  evaluateFull(bot);
}

std::string
Evaluator::info() const
{
  // Training specification
  std::string_view trainingSchedule {"First drag drop"};
  std::string_view finalSchedule {"N episodes"}; // TODO not yet true
  std::string_view statsCollection {"Dragging N/1000 ... N"};
  std::string s {};
  utils::append_more(s, trainingSchedule, " training with ", finalSchedule,
      " evaluation, stats=", statsCollection, "; ");

  // Parameters and results
  // TODO provide total performed training episodes
  utils::append_more(s,
      "N=", std::to_string(trainingEpisodes),
      " eval_mean=", std::to_string(finalStats.getMean()),
      " confidence=", std::to_string(finalStats.getRoundaboutConfidence()),
      " last_training_mean=", std::to_string(runningStats.getStats().back().getMean()),
      " confidence=", std::to_string(runningStats.getStats().back().getRoundaboutConfidence())
      );

  return s;
}

void defaultEvaluation()
{
  // Read parameters from stdin
  std::cin.sync(); std::cin.clear();
  double a, e;
  unsigned long n;
  std::cin >> n >> a >> e;
  std::cin.sync(); std::cin.clear();

  auto bot = new Sarsa(a, e);
  rl::Evaluator eval(n, n/10); // TODO eventually use n instead of n/10, but is too slow for testing
  eval(*bot);

  std::string const indexFileName {"eval/index0.txt"};
  std::ofstream tmpIndex(indexFileName, std::ios::trunc | std::ios::out);

  // TODO This file operation should probably be its own function
  if (!tmpIndex.is_open())
  {
    throw std::runtime_error(std::string() +
        "[ERROR] zehntausend: Could not open \"" + indexFileName +
        "to write info string");
  }
  else
  {
    std::cout << "Opened file " << indexFileName << " to write eval info." << std::endl;
  }
  std::string infoStr {"0 "}; // ID of temporary eval run
  utils::append_more(infoStr, "[Algo] ", bot->info(), " [Eval] ", eval.info());
  if (!(tmpIndex << infoStr))
  {
    if (tmpIndex.fail())
    {
      // File stream died
      if (tmpIndex.bad()) throw std::runtime_error(std::string() +
          "[ERROR] zehntausend: Index file fstream corrupt.");
      // Operation failed but stream lives
      else throw std::runtime_error(std::string() +
          "[ERROR] zehntausend: Index file writing failed.");
    }
    else
    {
      // TODO Can happen?
      std::cerr << "[WARNING] zehntausend: Unknown write operation fail."
                << std::endl;
    }
  }
  else
  {
    std::cout << "Index file written successfully" << std::endl;
  }
}

} // namespace rl
