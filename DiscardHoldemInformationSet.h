#ifndef DiscardHoldemInformationSet_1484584494112186
#define DiscardHoldemInformationSet_1484584494112186

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

class DiscardHoldemInformationSet {
 public:
  DiscardHoldemInformationSet(int player, std::pair<Card,Card> hand);
  void setHand(std::pair<Card,Card> hand);
  void setBoard(std::vector<Card> board);
  void makeMove(std::string action);
  int boardSize();
  std::string id(); 
 private:
  int player;
  std::pair<Card,Card> hand;
  std::vector<Card> board;
  std::vector<std::string> history;
  char ranks[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
  char suits[4] = {'c','d','h','s'};  
};

#endif
