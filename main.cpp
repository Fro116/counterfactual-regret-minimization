#include "RPSPayoutSet.h"
#include "KuhnPokerPayoutSet.h"
#include "CounterFactualRegretMinimizer.h"

/**
Uncomment this code to run the poker examples. Consult the readme for more information

#include "DiscardHoldemPayoutSet.h"
#include "DiscardHoldemInformationSet.h"
#include "DiscardHoldemBucketer.h"
*/

void solveRPS(std::string outputFile) {
  std::shared_ptr<PayoutSet<std::string, RPSInformationSet>> game(new RPSPayoutSet);
  CounterFactualRegretMinimizer<std::string, RPSInformationSet> trainer(game);
  trainer.solve(outputFile, 100000, 10000);
}

void solveKuhnPoker(std::string outputFile) {
  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> game(new KuhnPokerPayoutSet);
  CounterFactualRegretMinimizer<std::string, KuhnPokerInformationSet> trainer(game);
  trainer.solve(outputFile, 100000, 10000);
}

/** 
Uncomment this code to run the poker examples. Consult the readme for more information

void solveDiscardPoker() {
  DiscardHoldemBucketer::initDiscards("data/FlopDiscards.txt");
  std::cout << "LOADED FLOP DISCARDS:" << std::endl;
  DiscardHoldemBucketer::initDiscards("data/TurnDiscards.txt");
  std::cout << "LOADED TURN DISCARDS:" << std::endl;
  DiscardHoldemBucketer::initBuckets("data/FlopAssignments.txt");
  std::cout << "LOADED FLOP BUCKETS:" << std::endl;
  DiscardHoldemBucketer::initBuckets("data/TurnAssignments.txt");
  std::cout << "LOADED TURN BUCKETS:" << std::endl;
  DiscardHoldemBucketer::initCenters("data/RiverCenters.txt");
  std::cout << "LOADED RIVER BUCKETS:" << std::endl;
  std::shared_ptr<PayoutSet<std::string, DiscardHoldemInformationSet>> game(new DiscardHoldemPayoutSet);
  CounterFactualRegretMinimizer<std::string, DiscardHoldemInformationSet> trainer(game);
  trainer.solve("strategy.txt", 10000, 1000);
}
*/

/**                                                                                                                                                                                                             
 * Example usage of performing CFR. The following code solves the game of Kuhn poker                                                                                                                            
 * (https://en.wikipedia.org/wiki/Kuhn_poker) and saves the  strategy in the file                                                                                                                               
 * "results.txt".                                                                                                                                                                                               
 */

int main(int argc, char *argv[]) {
  srand(time(NULL));
  solveKuhnPoker("strategy.txt");
  return 0;
}
