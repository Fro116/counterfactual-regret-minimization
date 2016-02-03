#include "CounterFactualRegretMinimizer.h"

CounterFactualRegretMinimizer::CounterFactualRegretMinimizer(std::shared_ptr<PayoutSet<S,T>> payout) :
  payout(payout),
  random(),
  numPlayers(payout.numPlayers()),
  regrets(),
  strategies()
{

}

void CounterFactualRegretMinimizer::train(int iterations) {

}

std::vector<double> CounterFactualRegretMinimizer::strategyProfile(int player, T set) {
  std::vector<double> a;
  return a;
}

T CounterFactualRegretMinimizer::chooseMove(std::vector<double> strategy, std::vector<T> actions) {
  T why;
  return why;
}
