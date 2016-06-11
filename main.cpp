#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <thread>

#include "KuhnPokerPayoutSet.h"
#include "ParallelTestPayoutSet.h"
#include "Random.h"
#include "CounterFactualRegretMinimizer.h"

void train() {
  // std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  // CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game);
  //trainer.load("tmp.txt");
  std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> game(new ParallelTestPayoutSet);
  CounterFactualRegretMinimizer<std::string, ParallelTestInformationSet> trainer(game);
  trainer.train("results.txt", 2, 0.1, 100000, 10000, 1000);
  // for (int i = 0; i < 2; ++i) {
  //   std::cout << "ITERATION " << i << std::endl;
  //   trainer.train(10000, 1);
  // }
  // trainer.save("tmp.txt");
}

int main(int argc, char *argv[]) {
  train();
  //  trainer.load("tmp");
  //  trainer.save("tmp");

  return 0;
}
