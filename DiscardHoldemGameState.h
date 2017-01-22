#ifndef DiscardHoldemGameState_1484529653179970
#define DiscardHoldemGameState_1484529653179970

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

#include "Card.h"

class DiscardHoldemGameState {
  friend class DiscardHodlemPayoutSet;
  public:
    DiscardHoldemGameState();
    void beginGame();
    std::vector<std::string> actions();
    void makeMove(std::string action);
    std::pair<double, double> payout();    
    //  private:
    Card getCard();
    void placeChips(int player, int amount);    
    std::vector<Card> deck;
    std::pair<Card, Card> p1Hand;
    std::pair<Card, Card> p2Hand;
    std::vector<Card> board;
    int startingStack;    
    int p1Chips;
    int p2Chips;
    int pot;
    bool folded;
    std::vector<std::string> history;
    std::vector<std::string> roundHistory;
    std::string p1key;
    std::string p2key;    
    int playerToAct;
    bool isTerminalState;
    int turn;
    static std::string lastShowdown;
    static double lastShowdownEquity;      
};

#endif
