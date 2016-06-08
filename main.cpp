#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

#include "KuhnPokerPayoutSet.h"
#include "ParallelTestPayoutSet.h"
#include "Random.h"
#include "CounterFactualRegretMinimizer.h"

int main(int argc, char *argv[]) {
  Random rand;
  //  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> game(new ParallelTestPayoutSet);
  CounterFactualRegretMinimizer<std::string, ParallelTestInformationSet> trainer(game,true);
  trainer.train(100000,4);
  trainer.saveStrategy("tmp");
  return 0;
}
