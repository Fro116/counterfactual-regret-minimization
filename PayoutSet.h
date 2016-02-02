#ifndef __PayoutSet__1454376039745180__
#define __PayoutSet__1454376039745180__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>

/** S is the type of actions. T is the type of informationSets **/
template <typename S, typename T>
class PayoutSet {
 public:
  PayoutSet();
  virtual std::vector<double> value() = 0;
  virtual std::vector<T> beginGame() = 0;
  virtual std::vector<T> makeMove(std::vector<T> sets, S action) = 0;
  virtual bool isTerminalSet() = 0;
  virtual int numPlayers() = 0;
  virtual int playerToAct() = 0;
  virtual std::vector<S> actions() = 0;

 private:

};

#endif
