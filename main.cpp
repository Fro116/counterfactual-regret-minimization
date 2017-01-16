#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <thread>

#include "KuhnPokerPayoutSet.h"
#include "RPSPayoutSet.h"
#include "CounterFactualRegretMinimizer.h"

void train() {
  // std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  // CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game);
  //trainer.load("tmp.txt");
  // std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> game(new ParallelTestPayoutSet);
  // CounterFactualRegretMinimizer<std::string, ParallelTestInformationSet> trainer(game);
  // trainer.solve("results.txt", 2, 0.01, 1000000, 10000, 1000);
  // for (int i = 0; i < 2; ++i) {
  //   std::cout << "ITERATION " << i << std::endl;
  std::shared_ptr<PayoutSet<std::string, RPSInformationSet>> game(new RPSPayoutSet);
  CounterFactualRegretMinimizer<std::string, RPSInformationSet> trainer(game);  
  trainer.train(100000,2,1000);
  // }
  trainer.save("tmp.txt");
}

int main(int argc, char *argv[]) {
  train();
  //  trainer.load("tmp");
  //  trainer.save("tmp");

  return 0;
}
