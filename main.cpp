#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

#include "TicTacToePayoutMatrix.h"
#include "RegretMinimizer.h"
#include "KuhnPokerPayoutSet.h"
#include "Random.h"
#include "CounterFactualRegretMinimizer.h"

int main(int argc, char *argv[]) {
  Random rand;
  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game);
  trainer.train(100000,2);
  trainer.saveStrategy("tmp");
  return 0;
}
