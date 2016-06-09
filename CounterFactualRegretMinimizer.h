#ifndef __CounterFactualRegretMinimizer__1454457269771660__
#define __CounterFactualRegretMinimizer__1454457269771660__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "Random.h"

#include "PayoutSet.h"

/**S is the type of actions. T is the type of informationSets. **/
template <class S, class T>
class CounterFactualRegretMinimizer {
 public:
  CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout, bool display = false);
  void train(int iterations, int numThreads);
  void save(std::string filename);
  void load(std::string filename);
 private:
  void train(int iterations, std::vector<std::unordered_map<std::string,std::vector<double>>> regrets, std::vector<std::unordered_map<std::string,std::vector<double>>> strategies);
  std::vector<double> strategyProfile(int player, std::string id, std::vector<S> actions, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies);
  int chooseMove(std::vector<double>& strategy);
  std::vector<double> train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double>& factual, std::vector<double>& counterfactual, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies);
  
  std::shared_ptr<PayoutSet<S, T>> payout;
  Random random;
  int numPlayers;
  bool display;
  int outputPeriod;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregate_regrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregate_strategies;
  std::mutex stream_mutex;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout, bool display) :
  payout(payout),
  random(),
  numPlayers(payout->numPlayers()),
  aggregate_regrets(),
  aggregate_strategies(),
  display(display)
{
  for (int i = 0; i < numPlayers; ++i) {
    aggregate_regrets.push_back(std::unordered_map<std::string,std::vector<double>>());
    aggregate_strategies.push_back(std::unordered_map<std::string,std::vector<double>>());
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations, int numThreads) {
  std::vector<std::thread> threads;
  int iters = iterations / numThreads;
  outputPeriod = std::min(1000, iters / 1000);
  for (int thread = 0; thread < numThreads; ++thread) {
    threads.push_back(std::thread([this, iters](){train(iters, aggregate_regrets, aggregate_strategies);}));
  }
  for (std::thread& thr : threads) {
    thr.join();
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations, std::vector<std::unordered_map<std::string,std::vector<double>>> regrets, std::vector<std::unordered_map<std::string,std::vector<double>>> strategies) {
  std::shared_ptr<PayoutSet<S, T>> payoutCopy = payout->deepCopy();
  for (int i = 0; i < iterations; ++i) {
    if (display && i % outputPeriod == 0) {
      stream_mutex.lock();
      std::cout << "Iteration " << i << "\n";
      stream_mutex.unlock();
    }
    payoutCopy->beginGame();
    std::vector<double> factual;
    std::vector<double> counterfactual;
    for (int j = 0; j < numPlayers; ++j) {
      factual.push_back(1);
      counterfactual.push_back(1);
    }
    train(payoutCopy, factual, counterfactual, regrets, strategies);
  }
  aggregate_regrets = regrets;
  aggregate_strategies = strategies;
}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double>& factual, std::vector<double>& counterfactual, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies) {
  if (payouts->isTerminalState()) {
    return payouts->payout();
  }
  //Find the strategy profile
  std::vector<T> sets = payouts->infoSets();
  int player = payouts->playerToAct();
  std::vector<S> actions = payouts->actions();
  std::string id = payouts->uniqueIdentifier(sets[player]);
  std::vector<double> curStrategy = strategyProfile(player, id, actions, regrets, strategies);
  {
    std::vector<double>& cumulativeStrategy = strategies[player][id];
    while (actions.size() > cumulativeStrategy.size()) {
      cumulativeStrategy.push_back(0);
    }
    for (int i = 0; i < cumulativeStrategy.size(); ++i) {
      cumulativeStrategy[i] += curStrategy[i] * factual[player];
    }
  }
  //Calculate utilities
  std::vector<double> values;
  values.reserve(actions.size());
  std::vector<double> outcome;
  outcome.reserve(numPlayers);
  for (int i = 0; i < numPlayers; ++i) {
    outcome.push_back(0);
  }
  for (int i = 0; i < actions.size(); ++i) {
    std::shared_ptr<PayoutSet<S, T>> copy = payouts->deepCopy();
    copy->makeMove(actions[i]);
    std::vector<double> newCounterfactual = counterfactual;
    for (int index = 0; index < newCounterfactual.size(); ++index) {
      if (index != player) {
	newCounterfactual[index] *= curStrategy[i];
      }
    }
    std::vector<double> newFactual = factual;
    newFactual[player] *= curStrategy[i];
    std::vector<double> results = train(copy, newFactual, newCounterfactual, regrets, strategies); //forward passing of history and backward passing of rewards
    double moveValue = results[player];
    values.push_back(moveValue);
    for (int index = 0; index < results.size(); ++index) { 
      outcome[index] += results[index] * curStrategy[i];
    }
  }
  //Compute regrets
  {
    std::vector<double>& cumulativeRegrets = regrets[player][id];
    while (actions.size() > cumulativeRegrets.size()) {
      cumulativeRegrets.push_back(0);
    }
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      cumulativeRegrets[i] += (values[i] - outcome[player]) * counterfactual[player];
      if (cumulativeRegrets[i] < 0) {
	cumulativeRegrets[i] = 0;
      }
    }
  }
  return outcome;
}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::strategyProfile(int player, std::string id, std::vector<S> actions, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies) {
  std::vector<double> strategy;
  std::vector<double>& cumulativeRegrets = regrets[player][id];
  //If first time encountering set
  while (actions.size() > cumulativeRegrets.size()) {
    cumulativeRegrets.push_back(0);
  }
  double totalRegrets = 0;
  for (double regret : cumulativeRegrets) {
    totalRegrets += regret;
  }
  if (totalRegrets == 0) {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      strategy.push_back(1.0/cumulativeRegrets.size());
    }
  }
  else {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      strategy.push_back(cumulativeRegrets[i]/totalRegrets);
    }
  }
  return strategy;
}

template <class S, class T>
int CounterFactualRegretMinimizer<S, T>::chooseMove(std::vector<double>& strategy) {
  double value = random.real(0, 1);
  double prob = 0;
  for (int i = 0; i < strategy.size(); ++i) {
    prob += strategy[i];
    if (value < prob) {
      return i;
    }
  }
  return strategy.size() - 1;
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::save(std::string filename) {
  std::ofstream file(filename);
  file << "STRATEGIES" << "\n";
  for (int i = 0; i < numPlayers; ++i) {
    file << "PLAYER: " << i << std::endl;
    for (auto it = aggregate_strategies[i].begin(); it != aggregate_strategies[i].end(); ++it) {
      file << it->first;
      for (double value : it->second) {
	file << " " << value;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }
  file << "REGRETS" << "\n";
  for (int i = 0; i < numPlayers; ++i) {
    file << "PLAYER: " << i << std::endl;
    for (auto it = aggregate_regrets[i].begin(); it != aggregate_regrets[i].end(); ++it) {
      file << it->first;
      for (double value : it->second) {
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
  aggregate_strategies.clear();
  for (int player = 0; player < numPlayers; ++player) {
    aggregate_strategies.push_back(std::unordered_map<std::string,std::vector<double>>());
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
      aggregate_strategies[player][key] = value;
      std::getline(file, buffer);
    }
  }
  std::getline(file, buffer);
  if (buffer != "REGRETS") {
    return;
  }
  aggregate_regrets.clear();
  for (int player = 0; player < numPlayers; ++player) {
    aggregate_regrets.push_back(std::unordered_map<std::string,std::vector<double>>());
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
      aggregate_regrets[player][key] = value;
      std::getline(file, buffer);
    }
  }
}

#endif
