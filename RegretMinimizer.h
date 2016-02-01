#ifndef __RegretMinimizer__1454273623845551__
#define __RegretMinimizer__1454273623845551__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

#include "Random.h"
#include "PayoutMatrix.h"

class RegretMinimizer {
 public:
  RegretMinimizer(std::shared_ptr<PayoutMatrix> matrix);
  void train(int iterations);
  std::vector<double> strategyProfile(int player);
  std::vector<double> exploitability();
 private:
  int chooseMove(std::vector<double> strategy);
  bool permute(std::vector<int>& actions);
  std::shared_ptr<PayoutMatrix> payout;
  Random random;
  int numPlayers;
  std::vector<std::vector<double>> regrets;
  std::vector<std::vector<double>> strategies;
};

#endif
