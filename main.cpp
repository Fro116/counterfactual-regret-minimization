
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
  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game, false);
  trainer.train(25000,1);
}

int main(int argc, char *argv[]) {
  std::thread t1(train);
  std::thread t2(train);
  std::thread t3(train);
  std::thread t4(train);
  t1.join();
  t2.join();
  t3.join();
  t4.join();

  //std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> game(new ParallelTestPayoutSet);
  //CounterFactualRegretMinimizer<std::string, ParallelTestInformationSet> trainer(game,true);

  //  trainer.load("tmp");
  //  trainer.save("tmp");

  return 0;
}
