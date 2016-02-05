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
  trainer.train(100000);
  trainer.saveStrategy("tmp");
//   KuhnPokerPayoutSet hi;
//   hi.beginGame();
//   while (!hi.isTerminalState()) {
//   std::cout << "PLAYER: " << hi.playerToAct() << std::endl;
//   std::cout << "OPTIONS: " << std::endl;
//   for (std::string name : hi.actions()) {
//     std::cout << name << std::endl;
//   }
//   std::string option = hi.actions()[rand.integer(0, hi.actions().size()-1)];
//   hi.makeMove(option);
//   std::cout << "CHOICE: " << option << std::endl;
//   }
//   std::cout << "VALUE: " << hi.payout()[0] << " " << hi.payout()[1] << std::endl;
//   std::shared_ptr<PayoutMatrix> payout(new TicTacToePayoutMatrix);
//   RegretMinimizer trainer(payout);
//   trainer.train(1000000);
  return 0;
}
