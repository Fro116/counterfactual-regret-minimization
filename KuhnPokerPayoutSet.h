#ifndef __KuhnPokerPayoutSet__1454377138487349__
#define __KuhnPokerPayoutSet__1454377138487349__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>

#include "PayoutSet.h"

class KuhnPokerGameState;
class KuhnPokerInformationSet;

class KuhnPokerPayoutSet : public PayoutSet<std::string, KuhnPokerInformationSet> {
 public:
  KuhnPokerPayoutSet();
  std::vector<double> payout();
  void beginGame();
  void makeMove(std::string action);
  std::vector<KuhnPokerInformationSet> sets();
  std::shared_ptr<PayoutSet<std::string, KuhnPokerInformationSet>> deepCopy();
  bool isTerminalState();
  int numPlayers();
  int playerToAct();
  std::vector<std::string> actions();
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
 private:
  std::string card;
  int player;
  int pot;
  std::vector<std::string> history;
};

#endif
