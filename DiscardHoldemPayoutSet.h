#ifndef DiscardHoldemPayoutSet_1484532428999024
#define DiscardHoldemPayoutSet_1484532428999024

#include <string>
#include "PayoutSet.h"
#include "DiscardHoldemGameState.h"
#include "DiscardHoldemInformationSet.h"

/**
 * Class to encode discard poker as an abstract strategy game
 */
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
