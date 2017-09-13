#ifndef DiscardHoldemInformationSet_1484584494112186
#define DiscardHoldemInformationSet_1484584494112186

#include <string>
#include <vector>

#include "Card.h"

/**
 * Class to describe the information a player receives while
 * playing Discard poker
 */
class DiscardHoldemInformationSet {
 public:
  DiscardHoldemInformationSet(int player, std::pair<Card,Card> hand);
  void setHand(std::pair<Card,Card> hand);
  void setBoard(std::vector<Card> board);
  void makeMove(std::string action);
  int boardSize();
  std::string id();
  std::string key();   
 private:
  void updateName();
  int player;
  std::pair<Card,Card> hand;
  std::vector<Card> board;
  std::string history;
  std::string name;  
  std::string colex;
  static char ranks[];
  static char suits[];
  static int suitmap[];
};

#endif
