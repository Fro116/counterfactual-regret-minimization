#ifndef __ParallelTestPayoutSet__1454377138487349__
#define __ParallelTestPayoutSet__1454377138487349__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include "Random.h"

#include "PayoutSet.h"

class ParallelTestGameState;
class ParallelTestInformationSet;

class ParallelTestPayoutSet : public PayoutSet<std::string, ParallelTestInformationSet> {
 public:
  ParallelTestPayoutSet();
  std::vector<double> payout();
  void beginGame();
  void makeMove(std::string action);
  std::vector<ParallelTestInformationSet> infoSets();
  std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> deepCopy();
  bool isTerminalState();
  int numPlayers();
  int playerToAct();
  std::vector<std::string> actions();
  std::string uniqueIdentifier(ParallelTestInformationSet set);
 private:
  class ParallelTestGameState {
    friend class ParallelTestPayoutSet;
  public:
    ParallelTestGameState();
    std::vector<std::string> actions();
    void makeMove(std::string action);
  private:
    static Random rand;
    int p1Card;
    int p2Card;
    int pot;
    std::vector<std::string> history;
    std::vector<int> potContributions;
    int playerToAct;
    bool isTerminalState;
    int winningPlayer;
  };
  ParallelTestGameState gameState;
  std::vector<ParallelTestInformationSet> sets;
};

class ParallelTestInformationSet {
 public:
  ParallelTestInformationSet(int player, int card, int pot);
  void makeMove(std::string action, int pot);
  friend class ParallelTestPayoutSet;
 private:
  int card;
  int player;
  int pot;
  std::vector<std::string> history;
};



#endif
