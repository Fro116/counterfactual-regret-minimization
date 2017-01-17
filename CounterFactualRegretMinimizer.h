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
  /* void solve(std::string outputFile, int numThreads, double precision, long itersPerSave, long itersPerConvergenceTest, int itersPerThread = 1000); */
  void solve(std::string outputFile, int numThreads, long itersPerSave, long itersPerUpdate, int itersPerThread = 1000);  
  void train(long iterations, int numThreads, long itersPerThread = 1000);
  //itersPerThread is the number of iterations each thread should explore before merging their results
  void save(std::string filename);
  void load(std::string filename);
 private:
  void train(long iterations);
  std::vector<double> strategyProfile(int player, std::string id, std::vector<S>& actions, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets);
  int chooseMove(std::vector<double>& strategy);
  std::vector<double> train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double>& factual, std::vector<double>& counterfactual, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets, std::vector<std::unordered_map<std::string,std::vector<double>>>& strategies);
  void merge(std::vector<std::unordered_map<std::string,std::vector<double>>>& oldData, std::vector<std::unordered_map<std::string,std::vector<double>>>& newData);
  /* bool hasConverged(std::vector<std::unordered_map<std::string,std::vector<double>>>& aggregateData, std::vector<std::unordered_map<std::string,std::vector<double>>>& incrementalData, double precision); */
  
  std::shared_ptr<PayoutSet<S, T>> payout;
  Random random;
  int numPlayers;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateRegrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> aggregateStrategies;
  //For comparing new results to see if the algorithm converged
  /* std::vector<std::unordered_map<std::string,std::vector<double>>> incrementalStrategies; */
  std::mutex training_data_mutex;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout) :
  payout(payout),
  random(),
  numPlayers(payout->numPlayers()),
  aggregateRegrets(),
  aggregateStrategies()
  /*, incrementalStrategies() */
{
  for (int i = 0; i < numPlayers; ++i) {
    aggregateRegrets.push_back(std::unordered_map<std::string,std::vector<double>>());
    aggregateStrategies.push_back(std::unordered_map<std::string,std::vector<double>>());
    /* incrementalStrategies.push_back(std::unordered_map<std::string,std::vector<double>>()); */
  }
}


template <class S, class T>
/* void CounterFactualRegretMinimizer<S, T>::solve(std::string outputFile, int numThreads, double precision, long itersPerSave, long itersPerConvegenceTest, int itersPerThread) { */
void CounterFactualRegretMinimizer<S, T>::solve(std::string outputFile, int numThreads, long itersPerSave, long itersPerConvegenceTest, int itersPerThread) {    
  bool converged = false;
  long saveCounter = 0;
  long totalIterations = 0;
  std::cout << "BEGINNING TRAINING" << std::endl;
  while (!converged) {
    train(itersPerConvegenceTest, numThreads, itersPerThread);
    saveCounter += itersPerConvegenceTest;
    totalIterations += itersPerConvegenceTest;
    std::cout << "COMPLETED ITERATION: " << totalIterations << std::endl;
    /* converged = hasConverged(aggregateStrategies, incrementalStrategies, precision); */
    /* if (converged) { */
    /*   std::cout << "SAVING..." << std::endl; */
    /*   save(outputFile); */
    /*   break; */
    /* } */
    /* for (int player = 0; player < numPlayers; ++player) { */
    /*   for (auto& kv : incrementalStrategies[player]) { */
    /* 	kv.second.clear(); */
    /*   } */
    /* } */
    if (saveCounter >= itersPerSave) {
      saveCounter %= itersPerSave;
      std::cout << "SAVING..." << std::endl;
      save(outputFile);
    }
  }
}

/* template <class S, class T> */
/* bool CounterFactualRegretMinimizer<S, T>::hasConverged(std::vector<std::unordered_map<std::string,std::vector<double>>>& aggregateData, std::vector<std::unordered_map<std::string,std::vector<double>>>& incrementalData, double precision) { */
/*   double error = 0; */
/*   for (int player = 0; player < numPlayers; ++player) { */
/*     for (auto& kv : aggregateData[player]) { */
/*       if (incrementalData[player].find(kv.first) != incrementalData[player].end()) { */
/* 	std::vector<double>& incData = incrementalData[player][kv.first]; */
/* 	double aggregateTotal = 0; */
/* 	double previousTotal = 0; */
/* 	for (int index = 0; index < kv.second.size(); ++index) { */
/* 	  aggregateTotal += std::abs(kv.second[index]); */
/* 	  previousTotal += std::abs(kv.second[index] - incData[index]); */
/* 	} */
/* 	if (previousTotal == 0) { */
/* 	  return false; */
/* 	} */
/* 	for (int index = 0; index < kv.second.size(); ++index) { */
/* 	  error += std::abs(kv.second[index]/aggregateTotal - (kv.second[index] - incData[index])/previousTotal); */
/* 	  if (error > precision) { */
/* 	    return false; */
/* 	  } */
/* 	} */
/*       } */
/*     } */
/*   } */
/*   return true; */
/*   /\* std::cout << "ERROR: " << error << std::endl; *\/ */
/*   /\* return error < precision; *\/ */
/* } */

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(long iterations, int numThreads, long itersPerThread) {
  while (iterations > 0) {
    std::vector<std::thread> threads;
    for (int thread = 0; thread < numThreads; ++thread) {
      threads.push_back(std::thread([this, itersPerThread](){train(itersPerThread);}));
    }
    for (std::thread& thr : threads) {
      thr.join();
    }
    iterations -= itersPerThread * numThreads;
  }
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(long iterations) {
  std::shared_ptr<PayoutSet<S, T>> payoutCopy = payout->deepCopy();
  std::vector<std::unordered_map<std::string,std::vector<double>>> regrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> strategies;
  for (int i = 0; i < numPlayers; ++i) {
    regrets.push_back(std::unordered_map<std::string,std::vector<double>>());
    strategies.push_back(std::unordered_map<std::string,std::vector<double>>());
  }
  for (long i = 0; i < iterations; ++i) {
    payoutCopy->beginGame();
    std::vector<double> factual;
    std::vector<double> counterfactual;
    for (int j = 0; j < numPlayers; ++j) {
      factual.push_back(1);
      counterfactual.push_back(1);
    }
    train(payoutCopy, factual, counterfactual, regrets, strategies);
  }
  training_data_mutex.lock();
  /* merge(incrementalStrategies, strategies); */
  merge(aggregateRegrets, regrets);
  merge(aggregateStrategies, strategies);
  training_data_mutex.unlock();
}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::merge(std::vector<std::unordered_map<std::string,std::vector<double>>>& oldData, std::vector<std::unordered_map<std::string,std::vector<double>>>& newData) {
  for (int player = 0; player < numPlayers; ++player) {
    for (auto& it : newData[player]) {
      std::string key = it.first;
      std::vector<double>& value = it.second;
      while (oldData[player][key].size() < value.size()) {
	oldData[player][key].push_back(0);
      }
      for (int index = 0; index < value.size(); ++index) {
	oldData[player][key][index] += value[index];
      }
    }
  }
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
  std::vector<double> curStrategy = strategyProfile(player, id, actions, regrets);
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
std::vector<double> CounterFactualRegretMinimizer<S, T>::strategyProfile(int player, std::string id, std::vector<S>& actions, std::vector<std::unordered_map<std::string,std::vector<double>>>& regrets) {
  std::vector<double> strategy;
  strategy.reserve(actions.size());
  std::vector<double>& cumulativeRegrets = regrets[player][id];
  //If first time encountering set
  while (actions.size() > cumulativeRegrets.size()) {
    cumulativeRegrets.push_back(0);
  }
  //merge with stored results
  std::vector<double> history;
  history.reserve(actions.size());
  if (aggregateRegrets[player].find(id) != aggregateRegrets[player].end()) {
    for (double regret : aggregateRegrets[player][id]) {
      history.push_back(regret);
    }
  }
  else {
    while (actions.size() > history.size()) {
      history.push_back(0);
    }
  }
  double totalRegrets = 0;
  for (int i = 0; i < actions.size(); ++i) {
    double regret = cumulativeRegrets[i] + history[i];
    if (regret > 0) {
      totalRegrets += regret;
    }
  }
  if (totalRegrets == 0) {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      strategy.push_back(1.0/cumulativeRegrets.size());
    }
  }
  else {
    for (int i = 0; i < cumulativeRegrets.size(); ++i) {
      double regret = cumulativeRegrets[i] + history[i];
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
