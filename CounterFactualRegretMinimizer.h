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
#include <map>
#include "Random.h"

#include "PayoutSet.h"

/** S is the type of actions. T is the type of informationSets **/
template <class S, class T>
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
  std::vector<std::map<T,std::vector<double>>> regrets;
  std::vector<std::map<T,std::vector<double>>> strategies;
};


template <class S, class T>
CounterFactualRegretMinimizer<S, T>::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S,T>> payout) :
  payout(payout),
  random(),
  numPlayers(payout->numPlayers()),
  regrets(),
  strategies()
{

}

template <class S, class T>
void CounterFactualRegretMinimizer<S, T>::train(int iterations) {

}

template <class S, class T>
std::vector<double> CounterFactualRegretMinimizer<S, T>::strategyProfile(int player, T set) {
  std::vector<double> a;
  return a;
}

template <class S, class T>
T CounterFactualRegretMinimizer<S, T>::chooseMove(std::vector<double> strategy, std::vector<T> actions) {
  T why;
  return why;
}


#endif
