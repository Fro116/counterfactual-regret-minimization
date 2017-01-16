#include "DiscardHoldemPayoutSet.h"

DiscardHoldemPayoutSet::DiscardHoldemPayoutSet() :
  sets()
{

}

std::shared_ptr<PayoutSet<std::string, std::string>> DiscardHoldemPayoutSet::deepCopy() {
  std::shared_ptr<DiscardHoldemPayoutSet> copy(new DiscardHoldemPayoutSet);
  copy->gameState = gameState;
  copy->sets = sets;
  return copy;
}

std::string DiscardHoldemPayoutSet::uniqueIdentifier(std::string set) {
  return set;
}

std::vector<double> DiscardHoldemPayoutSet::payout() {
  std::vector<double> results;
  if (gameState.winningPlayer == 0) {
    results.push_back(gameState.pot + gameState.p1Chips-gameState.startingStack);
    results.push_back(gameState.p2Chips-gameState.startingStack);    
  }
  else if (gameState.winningPlayer == 1) {
    results.push_back(gameState.p1Chips-gameState.startingStack);
    results.push_back(gameState.pot + gameState.p2Chips-gameState.startingStack);        
  }
  return results;
}
  
void DiscardHoldemPayoutSet::beginGame() {
  gameState = DiscardHoldemGameState();
  sets.clear();
  std::string set1 = "P0";
  sets.push_back(set1);
  std::string set2("P1");
  sets.push_back(set2);
}

bool DiscardHoldemPayoutSet::isTerminalState() {
  return gameState.isTerminalState;
}

int DiscardHoldemPayoutSet::numPlayers() {
  return 2;
}

int DiscardHoldemPayoutSet::playerToAct() {
  return gameState.playerToAct;
}

std::vector<std::string> DiscardHoldemPayoutSet::actions() {
  return gameState.actions();
}

void DiscardHoldemPayoutSet::makeMove(std::string action) {
  gameState.makeMove(action);
  //TODO UPDATE INFOSETS
}

std::vector<std::string> DiscardHoldemPayoutSet::infoSets() {
  return sets;
}
