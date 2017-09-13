#ifndef DiscardHoldemGameState_1484529653179970
#define DiscardHoldemGameState_1484529653179970

#include <string>
#include <vector>

#include "Card.h"

/**
 * Class to encode the rules of poker
 */
class DiscardHoldemGameState {
  friend class DiscardHodlemPayoutSet;
  public:
    DiscardHoldemGameState();
    void beginGame();
    std::vector<std::string> actions();
    void makeMove(std::string action);
    std::pair<double, double> payout();    
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
