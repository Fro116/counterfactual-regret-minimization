#ifndef __CounterFactualRegretMinimizer__1454457269771660__
#define __CounterFactualRegretMinimizer__1454457269771660__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "PayoutSet.h"

/**
 * Template header-only library to perform Counterfactual Regret
 * Minimization on two player games.
 *
 * S is the type of actions. T is the type of informationSets. 
 */
template <class S, class T>
class CounterFactualRegretMinimizer {
 public:
  /**
   * Initializes the CFR algorithm
   */
  CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout);

  /**
   * Runs CFR for an infinite number of iterations. The computed strategy will 
   * periodically be saved to an output file.
   *
   * @param outputFile the file to save the current strategy to
   * @param itersPerSave the number of iterations before the strategy should be saved.
   * @param itersPerUpdate the number of iterations before a message is printed to
   *        the console to confirm that a program is running.
   */
  void solve(std::string outputFile, long itersPerSave, long itersPerUpdate);

  /**
   * Runs CFR for the given number of iterations
   *
   * @param iterations the number of iterations to run
   */
  void train(long iterations);

  /**
   * Saves the current strategy to the given file
   */
  void save(std::string filename);

  /**
   * Loads the strategy that was saved into the file. Training can be 
   * resumed without loss of infomation.
   */
  void load(std::string filename);
 private:
  std::vector<double> getStrategy(int player, std::string id, std::vector<S>& actions, double weight);

  /**
   * Analyzes one node of the game tree and recursively updates the strategy
   *
   * @param payouts the abstract strategy game to train
   * @param p9 the counterfactual probability that player 0 will cause this game state to occur
   * @param p1 the counterfactual probability that player 1 will cause this game state to occur
   */
  double train(std::shared_ptr<PayoutSet<S, T>> payouts, double p0, double p1);

  //! The abstract strategy game
  std::shared_ptr<PayoutSet<S, T>> payout;

  //! The number of players in the game
  int numPlayers;

  //! The accumulated regrets for each action in an action state over all iterations
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateRegrets;

  //! The number of times each action was selected from an action state over all iterations
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateStrategies;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout) :
  payout(payout),
  numPlayers(payout->numPlayers()),
  aggregateRegrets(),
  aggregateStrategies()
{
  for (int i = 0; i < numPlayers; ++i) {
    aggregateRegrets.push_back(std::unordered_map<std::string,std::vector<double>>());
    aggregateStrategies.push_back(std::unordered_map<std::string,std::vector<double>>());
  }
}


template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::solve(std::string outputFile, long itersPerSave, long itersPerConvegenceTest) {    
  bool converged = false;
  long saveCounter = 0;
  long totalIterations = 0;
  std::cout << "BEGINNING TRAINING" << std::endl;
  while (!converged) {
    train(itersPerConvegenceTest);
    saveCounter += itersPerConvegenceTest;
    totalIterations += itersPerConvegenceTest;
    std::cout << "COMPLETED ITERATION: " << totalIterations << std::endl;
    if (saveCounter >= itersPerSave) {
      saveCounter %= itersPerSave;
      std::cout << "SAVING..." << std::endl;
      save(outputFile);
    }
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(long iterations) {
  for (long i = 0; i < iterations; ++i) {
    std::shared_ptr<PayoutSet<S, T>> payoutCopy = payout->deepCopy();
    train(payoutCopy, 1, 1);
  }
}


template <class S, class T>
double CounterFactualRegretMinimizer<S, T>::train(std::shared_ptr<PayoutSet<S, T>> payouts, double p0, double p1) {
  //return payoff for terminal states
  int player = payouts->playerToAct();  
  if (payouts->isTerminalState()) {
    return payouts->payout()[0];
  }
  //create node if none exists
  std::vector<T> sets = payouts->infoSets();
  std::string id = payouts->uniqueIdentifier(sets[player]);
  auto actions = payouts->actions();  
  if (aggregateStrategies[player].find(id) == aggregateStrategies[player].end()) {
    std::vector<double> dummy;
    for (std::size_t i = 0; i < actions.size(); ++i) {
      dummy.push_back(0);
    }
    aggregateStrategies[player][id] = dummy;
    aggregateRegrets[player][id] = dummy;
  }
  //For each action, call cfr
  double weight;
  if (player == 0) {
    weight = p0;
  } else {
    weight = p1;
  }
  std::vector<double> strategy = getStrategy(player, id, actions, weight);
  std::vector<double> util;
  double nodeUtil = 0;
  for (std::size_t i = 0; i < actions.size(); ++i) {
    std::shared_ptr<PayoutSet<S, T>> copy = payouts->deepCopy();
    copy->makeMove(actions[i]);
    if (player == 0) {
      util.push_back(train(copy, p0*strategy[i], p1));
    } else {
      util.push_back(-train(copy, p0, p1*strategy[i]));
    }
    nodeUtil += strategy[i]*util[i];
  }
  //accumulate counterfactual regret
  for (std::size_t i = 0; i < actions.size(); ++i) {
    double regret = util[i] - nodeUtil;
    if (player == 0) {
      aggregateRegrets[player][id][i] += p1 * regret;
    }
    if (player == 1) {
      aggregateRegrets[player][id][i] += p0 * regret;
    }    
  }
  if (player == 0) {
    
    return nodeUtil;
  } else {
    return -nodeUtil;
  }
}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::getStrategy(int player, std::string id, std::vector<S>& actions, double weight) {
  std::vector<double> strategy;
  strategy.reserve(actions.size());
  std::vector<double>& cumulativeRegrets = aggregateRegrets[player][id];
  std::vector<double>& cumulativeStrategies = aggregateStrategies[player][id];  
  double normalizingSum = 0;
  for (int i = 0; i < actions.size(); ++i) {
    double regret;
    if (cumulativeRegrets[i]> 0) {
      regret = cumulativeRegrets[i];
    } else {
      regret = 0;
    }
    strategy.push_back(regret);
    normalizingSum += regret;
  }
  for (int i = 0; i < actions.size(); ++i) {
    if (normalizingSum > 0) {
      strategy[i] /= normalizingSum;
    } else {
      strategy[i] = 1.0/actions.size();
    }
    cumulativeStrategies[i] += weight * strategy[i];
  }
  return strategy;
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::save(std::string filename) {
  std::ofstream file(filename);
  file << "STRATEGIES" << "\n";
  for (int i = 0; i < numPlayers; ++i) {
    file << "PLAYER: " << i << std::endl;
    for (auto& it : aggregateStrategies[i]) {
      file << it.first;
      for (double value : it.second) {
	file << " " << value;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }
  file << "REGRETS" << "\n";
  for (int i = 0; i < numPlayers; ++i) {
    file << "PLAYER: " << i << std::endl;
    for (auto& it : aggregateRegrets[i]) {
      file << it.first;
      for (double value : it.second) {
	file << " " << value;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::load(std::string filename) {
  std::ifstream file(filename);
  std::string buffer;
  std::getline(file, buffer);
  if (buffer != "STRATEGIES") {
    return;
  }
  aggregateStrategies.clear();
  for (int player = 0; player < numPlayers; ++player) {
    aggregateStrategies.push_back(std::unordered_map<std::string,std::vector<double>>());
    std::getline(file, buffer); //discard line
    std::getline(file, buffer);
    while (buffer != "END") {
      std::stringstream line(buffer);
      std::string key;
      std::vector<double> value;
      line >> key;
      while (line.good()) {
	double entry;
	line >> entry;
	value.push_back(entry);
      }
      aggregateStrategies[player][key] = value;
      std::getline(file, buffer);
    }
  }
  std::getline(file, buffer);
  if (buffer != "REGRETS") {
    return;
  }
  aggregateRegrets.clear();
  for (int player = 0; player < numPlayers; ++player) {
    aggregateRegrets.push_back(std::unordered_map<std::string,std::vector<double>>());
    std::getline(file, buffer);
    std::getline(file, buffer); //discard line
    while (buffer != "END") {
      std::stringstream line(buffer);
      std::string key;
      std::vector<double> value;
      line >> key;
      while (line.good()) {
	double entry;
	line >> entry;
	value.push_back(entry);
      }
      aggregateRegrets[player][key] = value;
      std::getline(file, buffer);
    }
  }
}

#endif
