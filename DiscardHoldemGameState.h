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
    std::vector<std::string> actions();
    void makeMove(std::string action);
    //  private:
    Card GetCard();
    int Winner();
    std::vector<Card> deck;
    std::pair<Card, Card> p1Hand;
    std::pair<Card, Card> p2Hand;
    std::vector<Card> board;
    int startingStack;    
    int p1Chips;
    int p2Chips;
    int pot;
    int call;
    std::vector<std::string> history;
    int playerToAct;
    bool isTerminalState;
    int turn;
    int winningPlayer;  
};

#endif
