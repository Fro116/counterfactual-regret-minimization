#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

#include "TicTacToePayoutMatrix.h"
#include "RegretMinimizer.h"

int main(int argc, char *argv[]) {
  std::shared_ptr<PayoutMatrix> payout(new TicTacToePayoutMatrix);
  RegretMinimizer trainer(payout);
  trainer.train(10000);
  return 0;
}
