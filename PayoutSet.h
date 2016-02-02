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

class PayoutSet {
 public:
  using type = std::vector<std::string>>;
  PayoutSet();
  virtual std::vector<double> value() (type informationSet) = 0;
  virtual type initialSet() = 0;
  virtual bool isTerminalSet(type informationSet) = 0;
  virtual int numPlayers() = 0;
  virtual int playerToAct(type informationSet) = 0;
  virtual std::vector<type> actions(type informationSet) = 0;

 private:

};

#endif
