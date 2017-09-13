#include "RPSPayoutSet.h"
#include "KuhnPokerPayoutSet.h"
#include "DiscardHoldemPayoutSet.h"
#include "DiscardHoldemInformationSet.h"
#include "CounterFactualRegretMinimizer.h"

/**                                                                                                                                                                                                             
 * Example usage of performing CFR. The following code solves the game of Kuhn poker                                                                                                                            
 * (https://en.wikipedia.org/wiki/Kuhn_poker) and saves the  strategy in the file                                                                                                                               
 * "results.txt".                                                                                                                                                                                               
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

int main(int argc, char *argv[]) {
  srand(time(NULL));
  solveKuhnPoker("strategy.txt");
  return 0;
}
