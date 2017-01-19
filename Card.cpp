#include "Card.h"

Card::Card() : Card(0,0) {

}

Card::Card(int rank, int suit) : r(rank), s(suit) {
  char ranks[] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
  char suits[] = {'c','d','h','s'};
  str = "";
  str += ranks[rank];
  str += suits[suit];
}

std::string Card::toString() {
  return str;
}

int Card::rank() {
  return r;
}

int Card::suit() {
  return s;
}

bool Card::operator==(const Card &c1) const {
    return (c1.r == r && c1.s == s);
  }
