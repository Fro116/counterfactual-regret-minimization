#include "TicTacToePayoutMatrix.h"

TicTacToePayoutMatrix::TicTacToePayoutMatrix() : payout{{0,-1,1},{1,0,-1},{-1,1,0}} {

}

std::vector<double> TicTacToePayoutMatrix::operator() (std::vector<int> indices) {
  assert(indices.size() == 2);
  int row = indices[0];
  int col = indices[1];
  assert(!(row >= 3 || row < 0 || col < 0 || col >=3));
  std::vector<double> results;
  results.push_back(payout[row][col]);
  results.push_back(payout[col][row]);
  return results;
}

int TicTacToePayoutMatrix::size() {
  return 3;
}

int TicTacToePayoutMatrix::numPlayers() {
  return 2;
}
