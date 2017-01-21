#include "DiscardHoldemPayoutSet.h"

DiscardHoldemPayoutSet::DiscardHoldemPayoutSet() :
  sets()
{

}

std::shared_ptr<PayoutSet<std::string, DiscardHoldemInformationSet>> DiscardHoldemPayoutSet::deepCopy() {
  std::shared_ptr<DiscardHoldemPayoutSet> copy(new DiscardHoldemPayoutSet);
  copy->gameState = gameState;
  copy->sets = sets;
  return copy;
}

std::string DiscardHoldemPayoutSet::uniqueIdentifier(DiscardHoldemInformationSet set) {
  std::string id = set.id()+":";
  for (std::string action : actions()) {
    id += action + "_";
  }
  id.pop_back();				
  return id;
}

std::vector<double> DiscardHoldemPayoutSet::payout() {
  std::vector<double> results;
  std::pair<double, double> payout = gameState.payout();
  results.push_back(payout.first);
  results.push_back(payout.second);  
  return results;
}
  
void DiscardHoldemPayoutSet::beginGame() {
  gameState = DiscardHoldemGameState();
  gameState.beginGame();
  sets.clear();
  DiscardHoldemInformationSet set1(0, gameState.p1Hand);
  sets.push_back(set1);
  DiscardHoldemInformationSet set2(1, gameState.p2Hand);
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
  for (DiscardHoldemInformationSet& set : sets) {
    set.makeMove(action);
    if (set.boardSize() < gameState.board.size()) {
      set.setBoard(gameState.board);
    }
  }
  sets[0].setHand(gameState.p1Hand);
  sets[1].setHand(gameState.p2Hand);
  gameState.p1key = sets[0].key();
  gameState.p2key = sets[1].key();  
}

std::vector<DiscardHoldemInformationSet> DiscardHoldemPayoutSet::infoSets() {
  return sets;
}

