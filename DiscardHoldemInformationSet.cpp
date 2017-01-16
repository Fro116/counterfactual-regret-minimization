#include "DiscardHoldemInformationSet.h"

DiscardHoldemInformationSet::DiscardHoldemInformationSet(int player, std::pair<Card, Card> hand) :
  player(player),
  hand(hand),
  board(),
  history()
{

}
void DiscardHoldemInformationSet::makeMove(std::string action) {
  history.push_back(action);
}

void DiscardHoldemInformationSet::setHand(std::pair<Card, Card> h) {
  hand = h;
}

void DiscardHoldemInformationSet::setBoard(std::vector<Card> b) {
  board = b;
}

int DiscardHoldemInformationSet::boardSize() {
  return board.size();
}

std::string DiscardHoldemInformationSet::id() {
  std::vector<std::tuple<int, int, int>> cards;    
  for (auto card : board) {
    cards.push_back(std::make_tuple(card.rank(),card.suit(), 1));
  }
  cards.push_back(std::make_tuple(hand.first.rank(),hand.first.suit(), 0));
  cards.push_back(std::make_tuple(hand.second.rank(),hand.second.suit(), 0));  
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {
      return std::get<0>(left) < std::get<0>(right);
    });
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {
      return std::get<2>(left) < std::get<2>(right);
    });
  std::unordered_map<int, int> suitMapping;
  int openSuit = 0;
  for (auto& card : cards) {
    if (suitMapping.find(std::get<1>(card)) == suitMapping.end()) {
      suitMapping[std::get<1>(card)] = openSuit;	      
      std::get<1>(card) = openSuit;
      openSuit++;
    } else {
      std::get<1>(card) = suitMapping[std::get<1>(card)];
    }
  }
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {
      return std::get<1>(left) < std::get<1>(right);
    });
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {
      return std::get<0>(left) < std::get<0>(right);
    });
  std::stable_sort(cards.begin(), cards.end(), [](auto &left, auto &right) {
      return std::get<2>(left) < std::get<2>(right);
    });
      
  std::string board = "";
  for (auto card : cards) {
    board += ranks[std::get<0>(card)];
    board += suits[std::get<1>(card)];	    
  }
  
  std::string id = "";
  id += board;
  for (std::string action : history) {
    id += " ";
    id += action;
  }
  return id;
}


