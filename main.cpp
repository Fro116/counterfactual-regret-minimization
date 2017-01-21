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
#include "DiscardHoldemPayoutSet.h"
#include "DiscardHoldemInformationSet.h"
#include "DiscardHoldemBucketer.h"
// #include "CounterFactualRegretMinimizer.h"
#include "CounterFactualRegretMinimizer2p.h"

void train() {
  srand(time(NULL));
  // std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  // CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game);
  // trainer.solve("results.txt", 100000, 10000);
  
  //trainer.load("tmp.txt");
  // std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> game(new ParallelTestPayoutSet);
  // CounterFactualRegretMinimizer<std::string, ParallelTestInformationSet> trainer(game);
  // for (int i = 0; i < 2; ++i) {
  //   std::cout << "ITERATION " << i << std::endl;
  // std::shared_ptr<PayoutSet<std::string, RPSInformationSet>> game(new RPSPayoutSet);

  std::shared_ptr<PayoutSet<std::string, DiscardHoldemInformationSet>> game(new DiscardHoldemPayoutSet);  
  CounterFactualRegretMinimizer<std::string, DiscardHoldemInformationSet> trainer(game);
  // trainer.solve("results.txt", 1, 10000, 1000);  
  trainer.solve("results.txt", 10000, 1000);
  // trainer.train(5000);  

  // trainer.train(100000,2,1000);
  // }
  // trainer.save("tmp.txt");
}

int main(int argc, char *argv[]) {
  DiscardHoldemBucketer::init("/Users/kundanc/Coding/pokerbots/data/FlopAssignments.txt");
  std::cerr << "LOADED FLOPS:" << std::endl;
  DiscardHoldemBucketer::init("/Users/kundanc/Coding/pokerbots/data/TurnAssignments.txt");
  std::cerr << "LOADED TURNS:" << std::endl;
  DiscardHoldemBucketer::initCenters("/Users/kundanc/Coding/pokerbots/data/RiverCenters.txt");
  std::cerr << "LOADED CENTERS:" << std::endl;    
  train();
  //  trainer.load("tmp");
  //  trainer.save("tmp");

  return 0;
}
