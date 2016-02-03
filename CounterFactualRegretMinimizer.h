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

/** S is the type of actions. T is the type of informationSets **/
template <typename S, typename T>
class CounterFactualRegretMinimizer {
 public:
  CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S,T>> payout);
  void train(int iterations);
 private:
  std::vector<double> strategyProfile(int player, T set);
  T chooseMove(std::vector<double> strategy, std::vector<T> actions);

  std::shared_ptr<PayoutSet<S,T>> payout;
  Random random;
  int numPlayers;
  std::vector<std::unordered_map<T,std::vector<std::double>>> regrets;
  std::vector<std::unordered_map<T,std::vector<std::double>>> strategies;
};

#endif
