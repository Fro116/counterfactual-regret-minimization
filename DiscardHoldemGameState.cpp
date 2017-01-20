#include "DiscardHoldemGameState.h"
#include "Random.h"

#include <pbots_calc.h>

std::string DiscardHoldemGameState::lastShowdown = "";
double DiscardHoldemGameState::lastShowdownEquity = 0;  


DiscardHoldemGameState::DiscardHoldemGameState() :
  pot(0),
  playerToAct(0),
  deck(),
  history(),
  roundHistory(),
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
    
    if (handstr+boardstr != lastShowdown) {
      std::string deadstr = "";
      Results* res = alloc_results();
      calc(const_cast<char*>(handstr.c_str()), const_cast<char*>(boardstr.c_str()), const_cast<char*>(deadstr.c_str()), 1000000, res);
      lastShowdownEquity = *(res->ev);
      free_results(res);      
      lastShowdown = handstr+boardstr;
    }
    double win = lastShowdownEquity;
    double p1pay = (p1Chips - startingStack) + pot*win;
    double p2pay = (p2Chips - startingStack) + pot*(1-win);

  
    return std::make_pair(p1pay,p2pay);
  }
}

std::vector<std::string> DiscardHoldemGameState::actions() {
  std::vector<std::string> response;
  int chips;
  if (playerToAct == 0) {
    chips = p1Chips;
  } else {
    chips = p2Chips;
  }
  if (turn == 0) {
    if (roundHistory.size() == 0) {
      response.push_back("RAISE167");
      response.push_back("CALL");
      response.push_back("FOLD");      
    }
    else if (roundHistory.size() == 1) {
      if (roundHistory[0] == "RAISE167") {
	response.push_back("RAISE100");	
	response.push_back("CALL");
	response.push_back("FOLD");
      } else if (roundHistory[0] == "CALL") {
	response.push_back("RAISE100");
	response.push_back("CHECK");
      }
    }
    else if (roundHistory.size() == 2) {
      if (roundHistory[0] == "RAISE167") {
	if (roundHistory[1] == "RAISE100") {
	  response.push_back("RAISE100");	  
	  response.push_back("CALL");
	  response.push_back("FOLD");	  
	}
      } else if (roundHistory[0] == "CALL") {
	if (roundHistory[1] == "RAISE100") {
	  response.push_back("RAISE100");	  
	  response.push_back("CALL");
	  response.push_back("FOLD");	  
	}
      } 
    }
    else if (roundHistory.size() == 3) {
      if (roundHistory[0] == "RAISE167") {
	if (roundHistory[1] == "RAISE100") {
	  if (roundHistory[2] == "RAISE100") {
	    response.push_back("CALL");
	    response.push_back("FOLD");
	  }
	}
      } else if (roundHistory[0] == "CALL") {
	if (roundHistory[1] == "RAISE100") {
	  if (roundHistory[2] == "RAISE100") {
	    response.push_back("CALL");
	    response.push_back("FOLD");	  
	  }
	}
      }
    }
  }
  else if (turn == 1) {
    response.push_back("CHECK");    
  }
  else if (turn == 2) {
    if (roundHistory.size() == 0) {
      response.push_back("BET066");
      response.push_back("CHECK");
    }
    else if (roundHistory.size() == 1) {
      if (roundHistory[0] == "BET066") {
  	response.push_back("RAISE150");	
  	response.push_back("CALL");
  	response.push_back("FOLD");
      } else if (roundHistory[0] == "CHECK") {
	response.push_back("BET066");
  	response.push_back("CHECK");
      }
    }
    else if (roundHistory.size() == 2) {
      if (roundHistory[0] == "BET066") {
	if (roundHistory[1] == "RAISE150") {
	  response.push_back("CALL");
	  response.push_back("FOLD");
	}
      } else if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {	
  	  response.push_back("RAISE150");
  	  response.push_back("FOLD");	  
  	}
      }
    }
    else if (roundHistory.size() == 3) {
      if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {
	  if (roundHistory[2] == "RAISE150") {
	    response.push_back("CALL");
	    response.push_back("FOLD");
	  }
  	}
      }
    }    
  }
  else if (turn == 3) {
    response.push_back("CHECK");    
  }
  else if (turn == 4) {
    if (roundHistory.size() == 0) {
      response.push_back("BET066");
      response.push_back("CHECK");
    }
    else if (roundHistory.size() == 1) {
      if (roundHistory[0] == "BET066") {
  	response.push_back("RAISE150");	
  	response.push_back("CALL");
  	response.push_back("FOLD");
      } else if (roundHistory[0] == "CHECK") {
	response.push_back("BET066");
  	response.push_back("CHECK");
      }
    }
    else if (roundHistory.size() == 2) {
      if (roundHistory[0] == "BET066") {
	if (roundHistory[1] == "RAISE150") {
	  response.push_back("CALL");
	  response.push_back("FOLD");
	}
      } else if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {	
  	  response.push_back("RAISE150");
  	  response.push_back("FOLD");	  
  	}
      }
    }
    else if (roundHistory.size() == 3) {
      if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {
	  if (roundHistory[2] == "RAISE150") {
	    response.push_back("CALL");
	    response.push_back("FOLD");
	  }
  	}
      }
    }
  }
  else if (turn == 5) {
    if (roundHistory.size() == 0) {
      response.push_back("BET066");
      response.push_back("CHECK");
    }
    else if (roundHistory.size() == 1) {
      if (roundHistory[0] == "BET066") {
  	response.push_back("RAISE150");	
  	response.push_back("CALL");
  	response.push_back("FOLD");
      } else if (roundHistory[0] == "CHECK") {
	response.push_back("BET066");
  	response.push_back("CHECK");
      }
    }
    else if (roundHistory.size() == 2) {
      if (roundHistory[0] == "BET066") {
	if (roundHistory[1] == "RAISE150") {
	  response.push_back("CALL");
	  response.push_back("FOLD");
	}
      } else if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {	
  	  response.push_back("RAISE150");
  	  response.push_back("FOLD");	  
  	}
      }
    }
    else if (roundHistory.size() == 3) {
      if (roundHistory[0] == "CHECK") {
	if (roundHistory[1] == "BET066") {
	  if (roundHistory[2] == "RAISE150") {
	    response.push_back("CALL");
	    response.push_back("FOLD");
	  }
  	}
      }
    }
  }
  return response;
}

void DiscardHoldemGameState::makeMove(std::string action) {
  history.push_back(action);
  roundHistory.push_back(action);
  if (action.substr(0,3) == "BET") {
    int perc = std::stoi(action.substr(3));
    int amount = pot*(perc/100.0);    
    placeChips(playerToAct, amount);
    playerToAct = 1 - playerToAct;
  }  
  else if (action.substr(0,5) == "RAISE") {
    int perc = std::stoi(action.substr(5));
    int amount = pot*(perc/100.0);
    placeChips(playerToAct, amount);
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
    if (turn == 0) {
      int amount = call;
      placeChips(playerToAct, amount);      
      playerToAct = 1 - playerToAct;      
      if (roundHistory.size() > 1) {
	board.push_back(getCard());
	board.push_back(getCard());
	board.push_back(getCard());
	turn++;
	roundHistory.clear();
	playerToAct = 1;
      }
    } else if (turn == 2) {
	board.push_back(getCard());
	turn++;
	roundHistory.clear();
	playerToAct = 1;
    } else if (turn == 4) {
	board.push_back(getCard());
	turn++;
	roundHistory.clear();
	playerToAct = 1;
    } else if (turn == 5) {
	isTerminalState = true;
    } 
  }
  else if (action == "CHECK") {
    playerToAct = 1 - playerToAct;
    if (turn == 0) {
      board.push_back(getCard());
      board.push_back(getCard());
      board.push_back(getCard());
      turn++;
      roundHistory.clear();      
      playerToAct = 1;      
    } else if (turn == 1) {
      if (roundHistory.size() > 1) {
	turn++;
	roundHistory.clear();
	playerToAct = 1;
      }
    } else if (turn == 2) {
      if (roundHistory.size() > 1) {
	board.push_back(getCard());
	turn++;
	roundHistory.clear();
	playerToAct = 1;	
      }
    } else if (turn == 3) {
      if (roundHistory.size() > 1) {
	turn++;
	roundHistory.clear();
	playerToAct = 1;
      }
    } else if (turn == 4) {
      if (roundHistory.size() > 1) {
	board.push_back(getCard());
	turn++;
	roundHistory.clear();
	playerToAct = 1;	
      }      
    } else if (turn == 5) {
      if (roundHistory.size() > 1) {      
	isTerminalState = true;
      }
    }     
  }
  else if (action == "DISCARD") {
    playerToAct = 1 - playerToAct;
    if (roundHistory.size() > 1) {
      turn++;
      roundHistory.clear();
      playerToAct = 1;
    }
  }
}

void DiscardHoldemGameState::placeChips(int player, int amount) {
  if (player == 0) {
    amount = std::min(p1Chips, amount);
    p1Chips -= amount;
  } else {
    amount = std::min(p2Chips, amount);    
    p2Chips -= amount;
  }
  pot += amount;
  call = amount-call;
}
