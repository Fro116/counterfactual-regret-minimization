#include "RPSPayoutSet.h"

RPSPayoutSet::RPSPayoutSet() :
  sets()
{

}

void RPSPayoutSet::beginGame() {
  gameState = RPSGameState();
  sets.clear();
  RPSInformationSet set1(0);
  sets.push_back(set1);
  RPSInformationSet set2(1);
  sets.push_back(set2);  
}

std::shared_ptr<PayoutSet<std::string, RPSInformationSet>> RPSPayoutSet::deepCopy() {
  std::shared_ptr<RPSPayoutSet> copy(new RPSPayoutSet());
  copy->gameState = gameState;
  copy->sets = sets;
  return copy;
}

std::string RPSPayoutSet::uniqueIdentifier(RPSInformationSet set) {
  std::string hash;
  hash = std::to_string(set.player);
  return hash;
}

std::vector<double> RPSPayoutSet::payout() {
  assert(isTerminalState());
  std::vector<double> results;
  std::string first = gameState.history[0];
  std::string second = gameState.history[1];
  if ((first == "ROCK" && second == "SCISSORS") || (first == "SCISSORS" && second == "PAPER") || (first == "PAPER" && second == "ROCK")) {
    results.push_back(1);
    results.push_back(-1);
  }
  else if ((second == "ROCK" && first == "SCISSORS") || (second == "SCISSORS" && first == "PAPER") || (second == "PAPER" && first == "ROCK")) {
    results.push_back(-1);
    results.push_back(1);
  }
  else {
    results.push_back(0);
    results.push_back(0);
  }
  return results;
}
  
bool RPSPayoutSet::isTerminalState() {
  return gameState.history.size() == 2;
}

int RPSPayoutSet::numPlayers() {
  return 2;
}

int RPSPayoutSet::playerToAct() {
  return gameState.playerToAct;
}

std::vector<std::string> RPSPayoutSet::actions() {
  return gameState.actions();
}

void RPSPayoutSet::makeMove(std::string action) {
  gameState.makeMove(action);
}

std::vector<RPSInformationSet> RPSPayoutSet::infoSets() {
  return sets;
}

RPSPayoutSet::RPSGameState::RPSGameState() :
  playerToAct(0),
  history()
{

}

std::vector<std::string> RPSPayoutSet::RPSGameState::actions() {
  std::vector<std::string> response;
  response.push_back("ROCK");
  response.push_back("PAPER");
  response.push_back("SCISSORS");  
  return response;
}

void RPSPayoutSet::RPSGameState::makeMove(std::string action) {
  history.push_back(action);
  playerToAct = 1 - playerToAct;
}

RPSInformationSet::RPSInformationSet(int player) :
  player(player)
{

}

