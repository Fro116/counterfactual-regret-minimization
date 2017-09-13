#ifndef RPSPayoutSet_1484020875368595
#define RPSPayoutSet_1484020875368595

#include <string>

#include "PayoutSet.h"

class RPSPayoutSet;
class RPSInformationSet;

/**
 * Class to describe Rock-Paper-Scissors as an abstract strategy game.
 */

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
