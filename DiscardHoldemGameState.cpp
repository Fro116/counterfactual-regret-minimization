#include "DiscardHoldemGameState.h"

DiscardHoldemGameState::DiscardHoldemGameState() :
  pot(0),
  playerToAct(0),
  winningPlayer(-1),
  deck(),
  history(),
  startingStack(200),
  p1Chips(200),
  p2Chips(200),  
  isTerminalState(false),
  turn(0),
  board(),
  call(1)
{
  for (int rank = 0; rank < 13; ++rank) {
    for (int suit = 0; suit < 4; ++suit) {
      deck.push_back(Card(rank,suit));
    }
  }
  p1Hand = std::make_pair(GetCard(), GetCard());
  p2Hand = std::make_pair(GetCard(), GetCard());  
  p1Chips -= 1;
  p2Chips -= 2;
  pot += 3;
}

Card DiscardHoldemGameState::GetCard() {
  int index = std::rand() % deck.size();
  Card c = deck[index];
  deck.erase(deck.begin()+index);
  return c;
}

int Winner() {
  return 1;
}

std::vector<std::string> DiscardHoldemGameState::actions() {
  std::vector<std::string> response;  
  if (turn == 0) {
    if (history.size() == 0) {
      response.push_back("BET");
      response.push_back("CHECK");
      return response;
    }
    if (history.size() == 1 && history[0] == "CHECK") {
      response.push_back("BET");
      response.push_back("CHECK");
      return response;
    }
    if ((history.size() == 1 || history.size() == 2) && history[history.size()-1] == "BET") {
      response.push_back("CALL");
      response.push_back("FOLD");
      return response;
    }
  }
  return response;
}

void DiscardHoldemGameState::makeMove(std::string action) {
  history.push_back(action);
  if (turn == 0) {
    if (action == "BET") {
      if (playerToAct == 0) {
	p1Chips -= 3;
      } else {
	p2Chips -= 3;
      }
      pot += pot;
      call = 3-call;
      playerToAct = 1 - playerToAct;
    }
    else if (action == "CHECK" && history.size() == 1) {
      playerToAct = 1 - playerToAct;
    }
    else if (action == "CHECK" && history.size() > 1) {
      call = 0;
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());    
      isTerminalState = true;
      winningPlayer = Winner();
    }
    else if (action == "FOLD") {
      winningPlayer = 1 - playerToAct;
      isTerminalState = true;
    }
    else if (action == "CALL") {
      if (playerToAct == 0) {
	p1Chips -= call;
      } else {
	p2Chips -= call;
      }
      pot += call;
      call = 0;
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());
      board.push_back(GetCard());    
      isTerminalState = true;
      winningPlayer = Winner();
    }
  }
}

