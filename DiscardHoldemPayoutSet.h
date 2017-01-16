#ifndef DiscardHoldemPayoutSet_1484532428999024
#define DiscardHoldemPayoutSet_1484532428999024

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <chrono>
#include <set>
#include <unordered_set>
#include <cstdlib>
#include <tuple>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "PayoutSet.h"
#include "DiscardHoldemGameState.h"
#include "DiscardHoldemInformationSet.h"

class DiscardHoldemPayoutSet : public PayoutSet<std::string, DiscardHoldemInformationSet> {
 public:
  DiscardHoldemPayoutSet();
  std::vector<double> payout();
  void beginGame();
  void makeMove(std::string action);
  std::vector<DiscardHoldemInformationSet> infoSets();
  std::shared_ptr<PayoutSet<std::string, DiscardHoldemInformationSet>> deepCopy();
  bool isTerminalState();
  int numPlayers();
  int playerToAct();
  std::vector<std::string> actions();
  std::string uniqueIdentifier(DiscardHoldemInformationSet set);
 private:
  DiscardHoldemGameState gameState;
  std::vector<DiscardHoldemInformationSet> sets;
};

#endif
