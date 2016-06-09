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
#include <set>
#include <cstdlib>
#include "Random.h"

#include "PayoutSet.h"

/**S is the type of actions. T is the type of informationSets. **/
template <class S, class T>
class CounterFactualRegretMinimizer {
 public:
  CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout);
  void train(std::string filename, int numThreads, double precision = 0.01, int savePeriod = 10000, int threadPeriod = 1000);
  void train(int iterations, int numThreads);
  void save(std::string filename);
  void load(std::string filename);
 private:
  void train(int iterations, std::vector<std::unordered_map<std::string,std::vector<double>>> regrets, std::vector<std::unordered_map<std::string,std::vector<double>>> strategies);
  std::vector<double> strategyProfile(int player, std::string id, std::vector<S> actions, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies);
  int chooseMove(std::vector<double>& strategy);
  std::vector<double> train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double>& factual, std::vector<double>& counterfactual, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies);
  bool hasConverged(std::vector<std::unordered_map<std::string,std::vector<double>>>& oldStrategies, std::vector<std::unordered_map<std::string,std::vector<double>>>& newStrategies, double precision);
  
  std::shared_ptr<PayoutSet<S, T>> payout;
  Random random;
  int numPlayers;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateRegrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateStrategies;
  std::mutex stream_mutex;
  std::mutex training_data_mutex;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout) :
  payout(payout),
  random(),
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
void CounterFactualRegretMinimizer<S, T>::train(std::string filename, int numThreads, double precision, int savePeriod, int threadPeriod) {
  bool converged = false;
  int iteration = 0;
  int totalIterations = 0;
  std::cout << "BEGINNING TRAINING" << std::endl;
  while (!converged) {
    std::vector<std::unordered_map<std::string,std::vector<double>>> oldStrategies = aggregateStrategies;
    std::vector<std::unordered_map<std::string,std::vector<double>>> oldRegrets = aggregateRegrets;
    train(threadPeriod, numThreads);
    iteration += threadPeriod;
    totalIterations += threadPeriod;
    if (iteration >= savePeriod) {
      iteration %= savePeriod;
      save(filename);
      std::cout << "COMPLETED ITERATION: " << totalIterations << std::endl;
      converged = hasConverged(oldStrategies, aggregateStrategies, precision) && hasConverged(oldRegrets, aggregateRegrets, precision);
    }
  }
}

template <class S, class T>
bool CounterFactualRegretMinimizer<S, T>::hasConverged(std::vector<std::unordered_map<std::string,std::vector<double>>>& oldStrategies, std::vector<std::unordered_map<std::string,std::vector<double>>>& newStrategies, double precision) {
  double error = 0;
  for (int player = 0; player < numPlayers; ++player) {
    std::set<std::string> oldKeys;
    std::set<std::string> newKeys;
    for (auto kv : oldStrategies[player]) {
      oldKeys.insert(kv.first);
    }
    for (auto kv : newStrategies[player]) {
      newKeys.insert(kv.first);
    }
    if (oldKeys != newKeys) {
      return false;
    }
    for (auto key : oldKeys) {
      double oldTotal = 0;
      double newTotal = 0;
      for (int index = 0; index < oldStrategies[player][key].size(); ++index) {
	oldTotal += std::abs(oldStrategies[player][key][index]);
	newTotal += std::abs(newStrategies[player][key][index]);
      }
      for (int index = 0; index < oldStrategies[player][key].size(); ++index) {
	error += std::abs(oldStrategies[player][key][index]/oldTotal - newStrategies[player][key][index]/newTotal);
	//	if (error > precision) {
	//  return false;
	//}
      }
    }
  }
  //return true;
  std::cout << "ERROR: " << error << std::endl;
  return error < precision;
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations, int numThreads) {
  std::vector<std::thread> threads;
  int iters = iterations / numThreads;
  for (int thread = 0; thread < numThreads; ++thread) {
    training_data_mutex.lock();
    threads.push_back(std::thread([this, iters](){train(iters, aggregateRegrets, aggregateStrategies);}));
    training_data_mutex.unlock();
  }
  for (std::thread& thr : threads) {
    thr.join();
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations, std::vector<std::unordered_map<std::string,std::vector<double>>> regrets, std::vector<std::unordered_map<std::string,std::vector<double>>> strategies) {
  std::shared_ptr<PayoutSet<S, T>> payoutCopy = payout->deepCopy();
  std::vector<std::unordered_map<std::string,std::vector<double>>> regretsCopy = regrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> strategiesCopy = strategies;
  for (int i = 0; i < iterations; ++i) {
    payoutCopy->beginGame();
    std::vector<double> factual;
    std::vector<double> counterfactual;
    for (int j = 0; j < numPlayers; ++j) {
      factual.push_back(1);
      counterfactual.push_back(1);
    }
    train(payoutCopy, factual, counterfactual, regrets, strategies);
  }
  //Merge results
  for (int player = 0; player < numPlayers; ++player) {
    for (auto& it : regrets[player]) {
      std::string key = it.first;
      std::vector<double>& value = it.second;
      while (regretsCopy[player][key].size() < value.size()) {
	regretsCopy[player][key].push_back(0);
      }
      for (int index = 0; index < value.size(); ++index) {
	value[index] -= regretsCopy[player][key][index];
      }
    }
    for (auto& it : strategies[player]) {
      std::string key = it.first;
      std::vector<double>& value = it.second;
      while (strategiesCopy[player][key].size() < value.size()) {
	strategiesCopy[player][key].push_back(0);
      }
      for (int index = 0; index < value.size(); ++index) {
	value[index] -= strategiesCopy[player][key][index];
      }
    }
  }
  training_data_mutex.lock();
  for (int player = 0; player < numPlayers; ++player) {
    for (auto it : regrets[player]) {
      std::string key = it.first;
      std::vector<double> value = it.second;
      while (aggregateRegrets[player][key].size() < value.size()) {
	aggregateRegrets[player][key].push_back(0);
      }
      for (int index = 0; index < value.size(); ++index) {
	aggregateRegrets[player][key][index] += value[index];
      }
    }
    for (auto it : strategies[player]) {
      std::string key = it.first;
      std::vector<double> value = it.second;
      while (aggregateStrategies[player][key].size() < value.size()) {
	aggregateStrategies[player][key].push_back(0);
      }
      for (int index = 0; index < value.size(); ++index) {
	aggregateStrategies[player][key][index] += value[index];
      }
    }
  }
  training_data_mutex.unlock();
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
    if (regret < 0) {
      continue;
    }
    totalRegrets += regret;
  }
  if (totalRegrets == 0) {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      strategy.push_back(1.0/cumulativeRegrets.size());
    }
  }
  else {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      double regret = cumulativeRegrets[i];
      if (regret < 0) {
	regret = 0;
      }
      strategy.push_back(regret/totalRegrets);
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
    for (auto it : aggregateStrategies[i]) {
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
    for (auto it : aggregateRegrets[i]) {
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
