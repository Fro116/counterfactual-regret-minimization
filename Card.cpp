#include "Card.h"

std::string Card::ranks[] = {"2","3","4","5","6","7","8","9","T","J","Q","K","A"};
std::string Card::suits[] = {"c","d","h","s"};

Card::Card() : Card(0,0) {

}

Card::Card(int rank, int suit) : r(rank), s(suit) {

}

std::string Card::toString() {
  return ranks[r] + suits[s];
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
