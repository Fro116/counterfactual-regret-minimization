#ifndef __KuhnPokerPayoutSet__1454377138487349__
#define __KuhnPokerPayoutSet__1454377138487349__

#include <string>

#include "PayoutSet.h"

class KuhnPokerGameState;
class KuhnPokerInformationSet;

/**
 * Class to describle Kuhn poker as an abstract strategy game.
 */

class KuhnPokerPayoutSet : public PayoutSet<std::string, KuhnPokerInformationSet> {
 public:
  KuhnPokerPayoutSet();
  std::vector<double> payout();
  void makeMove(std::string action);
  std::vector<KuhnPokerInformationSet> infoSets();
  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> deepCopy();
  bool isTerminalState();
  int numPlayers();
  int playerToAct();
  std::vector<std::string> actions();
  std::string uniqueIdentifier(KuhnPokerInformationSet set);
 private:
  class KuhnPokerGameState {
    friend class KuhnPokerPayoutSet;
  public:
    KuhnPokerGameState();
    std::vector<std::string> actions();
    void makeMove(std::string action);
  private:
    std::string p1Card;
    std::string p2Card;
    int pot;
    std::vector<std::string> history;
    std::vector<int> potContributions;
    int playerToAct;
    bool isTerminalState;
    int winningPlayer;
  };
  KuhnPokerGameState gameState;
  std::vector<KuhnPokerInformationSet> sets;
};

class KuhnPokerInformationSet {
 public:
  KuhnPokerInformationSet(int player, std::string card, int pot);
  void makeMove(std::string action, int pot);
  friend class KuhnPokerPayoutSet;
 private:
  std::string card;
  int player;
  int pot;
  std::vector<std::string> history;
};

#endif
