#include "DiscardHoldemGameState.h"
#include "Random.h"

#include <pbots_calc.h>

DiscardHoldemGameState::DiscardHoldemGameState() :
  pot(0),
  playerToAct(0),
  deck(),
  history(),
  startingStack(200),
  p1Chips(200),
  p2Chips(200),  
  isTerminalState(false),
  folded(false),
  turn(0),
  board(),
  call(1)
{

}

void DiscardHoldemGameState::beginGame() {
  std::vector<Card> preshuffle;
  for (int rank = 0; rank < 13; ++rank) {
    for (int suit = 0; suit < 4; ++suit) {
      preshuffle.push_back(Card(rank,suit));
    }
  }
  for (int i = 0; i < 11; ++i) {
    int index = Random::integer(0, preshuffle.size()-1);
    Card c = preshuffle[index];
    preshuffle.erase(preshuffle.begin()+index);
    deck.push_back(c);
  }
  std::string s = "";
  for (auto c : deck) {
    s += c.toString();
  }
  p1Hand = std::make_pair(getCard(), getCard());
  p2Hand = std::make_pair(getCard(), getCard());
  p1Chips -= 1;
  p2Chips -= 2;
  pot += 3;
}

Card DiscardHoldemGameState::getCard() {
  Card c = deck[0];
  deck.erase(deck.begin());
  return c;
}

std::pair<double, double> DiscardHoldemGameState::payout() {
  if (folded) {
    double p1pay = (p1Chips - startingStack);
    double p2pay = (p2Chips - startingStack);
    return std::make_pair(p1pay,p2pay);
  } else {
    std::string handstr = "";
    handstr += p1Hand.first.toString()+p1Hand.second.toString();
    handstr += ":";
    handstr += p2Hand.first.toString()+p2Hand.second.toString();
  
    std::string boardstr = "";
    for (Card& c : board) {
      boardstr += c.toString();
    }
    std::string deadstr = "";

    Results* res = alloc_results();
    calc(const_cast<char*>(handstr.c_str()), const_cast<char*>(boardstr.c_str()), const_cast<char*>(deadstr.c_str()), 1000000, res);
    double win = *(res->ev);
    free_results(res);

    double p1pay = (p1Chips - startingStack) + pot*win;
    double p2pay = (p2Chips - startingStack) + pot*(1-win);  
  
    return std::make_pair(p1pay,p2pay);
  }
}

std::vector<std::string> DiscardHoldemGameState::actions() {
  std::vector<std::string> response;  
  if (turn == 0) {
    if (history.size() == 0) {
      response.push_back("RAISE");
      response.push_back("CALL");
      response.push_back("FOLD");      
      return response;
    }
    else if (history.size() == 1) {
      if (history[0] == "RAISE") {
	response.push_back("CALL");
	response.push_back("FOLD");
	return response;
      } else if (history[0] == "CALL") {
	response.push_back("RAISE");
	response.push_back("CHECK");
      }
    }
    else if (history.size() == 2) {
      if (history[0] == "CALL") {
	if (history[1] == "RAISE") {
	  response.push_back("CALL");
	  response.push_back("FOLD");	  
	}
      }
    }
  }
  return response;
}

void DiscardHoldemGameState::makeMove(std::string action) {
  history.push_back(action);
  if (turn == 0) {
    if (action == "RAISE") {
      int amount = 56;
      if (playerToAct == 0) {
	p1Chips -= amount;
      } else {
	p2Chips -= amount;
      }
      pot += amount;
      call = amount-call;
      playerToAct = 1 - playerToAct;
    }
    else if (action == "FOLD") {
      folded = true;
      int winningPlayer = 1 - playerToAct;
      if (winningPlayer == 0) {
	p1Chips += pot; 
      } else {
	p2Chips += pot;
      }
      isTerminalState = true;
    }        
    else if (action == "CALL") {
      int amount = call;
      if (playerToAct == 0) {
	p1Chips -= amount;
      } else {
	p2Chips -= amount;
      }
      pot += amount;
      call = 0;      
      playerToAct = 1 - playerToAct;      
      if (history.size() > 1) {
	board.push_back(getCard());
	board.push_back(getCard());
	board.push_back(getCard());
	board.push_back(getCard());
	board.push_back(getCard());    
	isTerminalState = true;
      }
    }
    else if (action == "CHECK") {
      call = 0;
      board.push_back(getCard());
      board.push_back(getCard());
      board.push_back(getCard());
      board.push_back(getCard());
      board.push_back(getCard());    
      isTerminalState = true;
    }
  }
}

