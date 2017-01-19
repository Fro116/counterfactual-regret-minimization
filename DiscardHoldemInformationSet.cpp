#include "DiscardHoldemInformationSet.h"

#include "DiscardHoldemBucketer.h"

char DiscardHoldemInformationSet::ranks[] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
char DiscardHoldemInformationSet::suits[] = {'c','d','h','s'};
int DiscardHoldemInformationSet::suitmap[4];

DiscardHoldemInformationSet::DiscardHoldemInformationSet(int player, std::pair<Card, Card> hand) :
  player(player),
  hand(hand),
  board(),
  history(),
  name()
{
  updateName();

}
void DiscardHoldemInformationSet::makeMove(std::string action) {
  history += "_"+action;
}

void DiscardHoldemInformationSet::setHand(std::pair<Card, Card> h) {
  if (h != hand) {
    hand = h;
    updateName();
  }
}

void DiscardHoldemInformationSet::setBoard(std::vector<Card> b) {
  if (b.size() != board.size()) {
    board = b;
    updateName();    
  } 
}

void DiscardHoldemInformationSet::updateName() {
  std::vector<std::tuple<int, int, int>> cards;    
  for (auto card : board) {
    cards.push_back(std::make_tuple(card.rank(),card.suit(), 1));
  }
  cards.push_back(std::make_tuple(hand.first.rank(),hand.first.suit(), 0));
  cards.push_back(std::make_tuple(hand.second.rank(),hand.second.suit(), 0));  
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {return std::get<0>(left) < std::get<0>(right);});
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {return std::get<2>(left) < std::get<2>(right);});
  int* writer = suitmap;
  for (int i = 0; i< 4; ++i) {
    *(writer++) = -1;
  }
  int openSuit = 0;
  for (auto& card : cards) {
    if (suitmap[std::get<1>(card)] == -1) {
      suitmap[std::get<1>(card)] = openSuit;	      
      std::get<1>(card) = openSuit;
      openSuit++;
    } else {
      std::get<1>(card) = suitmap[std::get<1>(card)];
    }
  }  
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {return std::get<1>(left) < std::get<1>(right);});
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {return std::get<0>(left) < std::get<0>(right);});
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {return std::get<2>(left) < std::get<2>(right);});
      
  std::string cardset = "";
  for (auto card : cards) {
    cardset += ranks[std::get<0>(card)];
    cardset += suits[std::get<1>(card)];	    
  }
  
  std::string id = "";

  if (board.size() == 0) {
    id += cardset;    
  } else if (board.size() == 3) {
    int bucket = DiscardHoldemBucketer::bucket(cardset);
    id += std::to_string(bucket);
  } 
  name = id;
}

int DiscardHoldemInformationSet::boardSize() {
  return board.size();
}

std::string DiscardHoldemInformationSet::id() {
  return name + history;
}
