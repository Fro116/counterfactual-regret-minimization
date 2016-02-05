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
#include "Random.h"

#include "PayoutSet.h"

/**S is the type of actions. T is the type of informationSets. **/
template <class S, class T>
class CounterFactualRegretMinimizer {
 public:
  CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout);
  void train(int iterations);
  void saveStrategy(std::string filename);
 private:
  std::vector<double> strategyProfile(int player, T set, std::vector<S> actions);
  int chooseMove(std::vector<double> strategy);
  std::vector<double> train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double> factual, std::vector<double> counterfactual);

  std::shared_ptr<PayoutSet<S, T>> payout;
  Random random;
  int numPlayers;
  std::vector<std::unordered_map<std::string,std::vector<double>>> regrets;
  std::vector<std::unordered_map<std::string,std::vector<double>>> strategies;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S, T>> payout) :
  payout(payout),
  random(),
  numPlayers(payout->numPlayers()),
  regrets(),
  strategies()
{
  for (int i = 0; i < numPlayers; ++i) {
    std::unordered_map<std::string,std::vector<double>> regret;
    std::unordered_map<std::string,std::vector<double>> strategy;
    regrets.push_back(regret);
    strategies.push_back(strategy);
  }

}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations) {
  for (int i = 0; i < iterations; ++i) {
    std::cout << "Iteration " << i << "\n";
    payout->beginGame();
    std::vector<double> factual;
    std::vector<double> counterfactual;
    for (int i = 0; i < numPlayers; ++i) {
      factual.push_back(1);
      counterfactual.push_back(1);
    }
    train(payout, factual, counterfactual);
  }
}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::train(std::shared_ptr<PayoutSet<S, T>> payouts, std::vector<double> factual, std::vector<double> counterfactual) {
  if (payouts->isTerminalState()) {
    return payouts->payout();
  }
  //Find the strategy profile
  std::cout << "Finding strategy profile " << "\n";
  std::vector<T> sets = payouts->infoSets();
  int player = payouts->playerToAct();
  std::vector<S> actions = payouts->actions();
  std::vector<double> curStrategy = strategyProfile(player, sets[player], actions);
  //Update cumulative strategy
  std::cout << "Updating strategy profile " << "\n";
  std::vector<double>& cumulativeStrategy = strategies[player][payout->uniqueIdentifier(sets[player])];
  //If first time encountering set
  while (actions.size() > cumulativeStrategy.size()) {
    cumulativeStrategy.push_back(0);
  }
  for (int i = 0; i < cumulativeStrategy.size(); ++i) {
    cumulativeStrategy[i] += curStrategy[i] * factual[player];
  }
  //Choose a move
  std::cout << "Choosing move " << "\n";
  int moveIndex = chooseMove(curStrategy);
  //Calculate utilities
  std::cout << "Calculating utilities " << "\n";
  std::vector<double> values;
  std::vector<double> outcome;
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
    std::vector<double> results = train(copy, newFactual, newCounterfactual); //forward passing of history and backward passing of rewards
    double moveValue = results[player];
    values.push_back(moveValue);
    for (int index = 0; index < results.size(); ++index) { 
      std::cout << "INDEX: " << results.size() << "\n";
      outcome[index] += results[index] * curStrategy[i];
    }
  }
  //Compute regrets
  std::cout << "Computing regrets " << "\n";
  std::vector<double>& cumulativeRegrets = regrets[player][payout->uniqueIdentifier(sets[player])];
  while (actions.size() > cumulativeRegrets.size()) {
    cumulativeRegrets.push_back(0);
  }
  for (int i = 0; i < cumulativeRegrets.size(); ++i) {
    cumulativeRegrets[i] += (values[i] - values[moveIndex]) * counterfactual[player];
    if (cumulativeRegrets[i] < 0) {
      cumulativeRegrets[i] = 0;
    }
  }
  return outcome;
}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::strategyProfile(int player, T set, std::vector<S> actions) {
  std::vector<double>& cumulativeRegrets = regrets[player][payout->uniqueIdentifier(set)];
  //If first time encountering set
  while (actions.size() > cumulativeRegrets.size()) {
    cumulativeRegrets.push_back(0);
  }
  int totalRegrets = 0;
  for (int regret : cumulativeRegrets) {
    totalRegrets += regret;
  }
  std::vector<double> strategy;
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
int CounterFactualRegretMinimizer<S, T>::chooseMove(std::vector<double> strategy) {
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
void CounterFactualRegretMinimizer<S, T>::saveStrategy(std::string filename) {
  std::ofstream file(filename);
  for (int i = 0; i < numPlayers; ++i) {
    file << "Player " << i << "\n";
    for (auto it = strategies[i].begin(); it != strategies[i].end(); ++it) {
      file << it->first << "\t";
      double total = 0;
      for (double value : it->second) {
	total += value;
      }
      for (double value : it->second) {
	file << value/total << " ";
      }
      file << "\n";
    }
  }
}

#endif
