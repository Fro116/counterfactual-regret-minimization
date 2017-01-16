#include "Card.h"

Card::Card() : Card(0,0) {

}

Card::Card(int rank, int suit) : rank(rank), suit(suit) {
  char ranks[] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
  char suits[] = {'c','d','h','s'};
  str = "";
  str += ranks[rank];
  str += suits[suit];
}

std::string Card::ToString() {
  return str;
}
