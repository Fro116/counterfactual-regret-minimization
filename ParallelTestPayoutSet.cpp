#include "ParallelTestPayoutSet.h"
#include "Random.h"

ParallelTestPayoutSet::ParallelTestPayoutSet() :
  sets()
{

}

std::shared_ptr<PayoutSet<std::string, ParallelTestInformationSet>> ParallelTestPayoutSet::deepCopy() {
  std::shared_ptr<ParallelTestPayoutSet> copy(new ParallelTestPayoutSet);
  copy->gameState = gameState;
  copy->sets = sets;
  return copy;
}

std::string ParallelTestPayoutSet::uniqueIdentifier(ParallelTestInformationSet set) {
  std::string hash;
  hash = std::to_string(set.card) + "_" + std::to_string(set.player) + "_" + std::to_string(set.pot);
  for (std::string str : set.history) {
    hash += "_" + str;
  }
  return hash;
}

std::vector<double> ParallelTestPayoutSet::payout() {
  assert(gameState.isTerminalState);
  std::vector<double> results;
  if (gameState.winningPlayer == 0) {
    results.push_back(gameState.pot - gameState.potContributions[0]);
    results.push_back(-gameState.potContributions[1]);
  }
  else if (gameState.winningPlayer == 1) {
    results.push_back(-gameState.potContributions[0]);
    results.push_back(gameState.pot - gameState.potContributions[1]);
  }
  return results;
}
  
void ParallelTestPayoutSet::beginGame() {
  gameState = ParallelTestGameState();
  sets.clear();
  ParallelTestInformationSet set1(0, gameState.p1Card, gameState.pot);
  sets.push_back(set1);
  ParallelTestInformationSet set2(1, gameState.p2Card, gameState.pot);
  sets.push_back(set2);
}

bool ParallelTestPayoutSet::isTerminalState() {
  return gameState.isTerminalState;
}

int ParallelTestPayoutSet::numPlayers() {
  return 2;
}

int ParallelTestPayoutSet::playerToAct() {
  return gameState.playerToAct;
}

std::vector<std::string> ParallelTestPayoutSet::actions() {
  return gameState.actions();
}

void ParallelTestPayoutSet::makeMove(std::string action) {
  gameState.makeMove(action);
  for (ParallelTestInformationSet& set : sets) {
    set.makeMove(action, gameState.pot);
  }
}

std::vector<ParallelTestInformationSet> ParallelTestPayoutSet::infoSets() {
  return sets;
}

ParallelTestPayoutSet::ParallelTestGameState::ParallelTestGameState() :
  pot(2),
  playerToAct(0),
  winningPlayer(-1),
  history(),
  potContributions(),
  isTerminalState(false)
{
  p1Card = 1;
  p2Card = 1;
  while (p1Card == p2Card) {
    p1Card = rand.integer(1,1000);
    p2Card = rand.integer(1,1000);
  }
  potContributions.push_back(1);
  potContributions.push_back(1);
}

std::vector<std::string> ParallelTestPayoutSet::ParallelTestGameState::actions() {
  if (history.size() == 0) {
    std::vector<std::string> response;
    response.push_back("BET");
    response.push_back("CHECK");
    return response;
  }
  if (history.size() == 1 && history[0] == "CHECK") {
    std::vector<std::string> response;
    response.push_back("BET");
    response.push_back("CHECK");
    return response;
  }
  if ((history.size() == 1 || history.size() == 2) && history[history.size()-1] == "BET") {
    std::vector<std::string> response;
    response.push_back("CALL");
    response.push_back("FOLD");
    return response;
  }
  std::vector<std::string> response;
  return response;
}

void ParallelTestPayoutSet::ParallelTestGameState::makeMove(std::string action) {
  history.push_back(action);
  if (action == "BET") {
    pot += 1;
    potContributions[playerToAct] += 1;
    playerToAct = 1 - playerToAct;
  }
  else if (action == "CHECK" && history.size() == 1) {
    pot += 0;
    playerToAct = 1 - playerToAct;
  }
  else if (action == "CHECK" && history.size() == 2) {
    pot += 0;
    winningPlayer = 1 - playerToAct;
    if (p1Card > p2Card) {
      winningPlayer = 0;
    }
    else {
      winningPlayer = 1;
    }
    isTerminalState = true;
  }
  else if (action == "FOLD") {
    pot += 0;
    winningPlayer = 1 - playerToAct;
    isTerminalState = true;
  }
  else if (action == "CALL") {
    potContributions[playerToAct] += 1;
    pot += 1;
    if (p1Card > p2Card) {
      winningPlayer = 0;
    }
    else {
      winningPlayer = 1;
    }
    isTerminalState = true;
  }
}

ParallelTestInformationSet::ParallelTestInformationSet(int player, int card, int pot) :
  player(player),
  pot(pot),
  card(card)
{

}

void ParallelTestInformationSet::makeMove(std::string action, int pot) {
  history.push_back(action);
  this->pot = pot;
}
