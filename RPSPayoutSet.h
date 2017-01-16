#ifndef RPSPayoutSet_1484020875368595
#define RPSPayoutSet_1484020875368595

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
class RPSPayoutSet;
class RPSInformationSet;

class RPSPayoutSet : public PayoutSet<std::string, RPSInformationSet> {
 public:
  RPSPayoutSet();
  std::vector<double> payout();
  void beginGame();
  void makeMove(std::string action);
  std::shared_ptr<PayoutSet<std::string, RPSInformationSet>> deepCopy();
  std::vector<RPSInformationSet> infoSets();
  bool isTerminalState();
  int numPlayers();
  int playerToAct();
  std::vector<std::string> actions();
  std::string uniqueIdentifier(RPSInformationSet set);  
 private:
  class RPSGameState {
  public:
    RPSGameState();
    std::vector<std::string> actions();
    void makeMove(std::string action);
    friend class RPSPayoutSet;    
  private:
    std::vector<std::string> history;
    int playerToAct;
  };
  RPSGameState gameState;
  std::vector<RPSInformationSet> sets;  
};

class RPSInformationSet {
 public:
  RPSInformationSet(int player);
  friend class RPSPayoutSet;
 private:
  int player;
};

#endif
