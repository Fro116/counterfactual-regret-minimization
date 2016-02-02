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

class KuhnPokerPayoutSet : public PayoutSet {
 public:
  KuhnPokerPayoutSet();
  std::vector<double> value();
  std::vector<KuhnPokerInformationSet> beginGame();
  std::vector<KuhnPokerInformationSet> makeMove(std::vector<KuhnPokerInformationSet> sets, std::string action);
  bool isTerminalSet();
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
    int playerToAct;
    bool isTerminalState;
    int winningPlayer;
  };
  KuhnPokerGameState gameState;
};

class KuhnPokerInformationSet {
 public:
  KuhnPokerInfomationSet(int player, std::string card, int pot);
  void makeMove(std::string action);
 private:
  std::string card;
  int player;
  int pot;
  std::vector<std::string> history;
};

#endif
