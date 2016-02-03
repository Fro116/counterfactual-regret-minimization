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

/** R is the specialized PayoutSet. S is the type of actions. T is the type of informationSets. **/
/** All instantiations of T must inject a hash function into std for use with std::unordered_map **/
/** All instantiations of T must overload the equality function **/
template <class S, class T>
class PayoutSet {
 public:
  virtual std::vector<double> payout() = 0;
  virtual void beginGame() = 0;
  virtual void makeMove(S action) = 0;
  virtual std::shared_ptr<PayoutSet<S, T>> deepCopy() = 0;
  virtual std::vector<T> infoSets() = 0;
  virtual bool isTerminalState() = 0;
  virtual int numPlayers() = 0;
  virtual int playerToAct() = 0;
  virtual std::vector<S> actions() = 0;
 private:

};

#endif
