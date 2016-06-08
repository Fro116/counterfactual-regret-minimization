#ifndef __TicTacToePayoutMatrix__1454269820468515__
#define __TicTacToePayoutMatrix__1454269820468515__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cassert>

#include "PayoutMatrix.h"

class TicTacToePayoutMatrix : public PayoutMatrix {
 public:
  TicTacToePayoutMatrix();
  std::vector<double> operator() (std::vector<int> indices);
  int size();
  int numPlayers();
 private:
  double payout[3][3];
};

#endif
