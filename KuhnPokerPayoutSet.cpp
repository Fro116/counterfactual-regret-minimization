#include "KuhnPokerPayoutSet.h"
#include "Random.h"

KuhnPokerPayoutSet::KuhnPokerPayoutSet() :
  KuhnPokerGameState()
{

}

std::vector<double> KuhnPokerPayoutSet::value() {
  assert(gameState.isTerminal);
  std::vector<double> results;
  if (gameState.winningPlayer == 0) {
    results.push_back(gameState.pot);
    results.push_back(0);
  }
  else if (gameState.winningPlayer == 1) {
    results.push_back(0);
    results.push_back(gameState.pot);
  }
  return results;
}
  
std::vector<KuhnPokerInformationSet> KuhnPokerPayoutSet::beginGame() {
  std::vector<KuhnPokerInformationSet>> sets;
  KuhnPokerInformationState set1(0, gameState.p1Card, gameState.pot);
  sets.push_back(set1);
  KuhnPokerInformationState set2(1, gameState.p2Card, gameState.pot);
  sets.push_back(set2);
  return sets;
}

bool KuhnPokerPayoutSet::isTerminalSet() {
  return gameState.isTerminalState;
}

int KuhnPokerPayoutSet::numPlayers() {
  return 2;
}

int KuhnPokerPayoutSet::playerToAct() {
  return gameState.playerToAct;
}

std::vector<std::string> KuhnPokerPayoutSet::actions() {
  return gameState.actions();
}

std::vector<KuhnPokerInformationSet> KuhnPokerPayoutSet::makeMove(std::vector<KuhnPokerInformationSet> sets, std::string action) {
  gameState.makeMove(action);
  for (KuhnPokerInformationSet set : informationSets) {
    set.makeMove(action, gameState.pot);
  }
  return sets;
}

KuhnPokerPayoutSet::KuhnPokerGameState::KuhnPokerGameState() :
  pot(2),
  playerToAct(0),
  winningPlayer(-1),
  history()
{
  Random rand;
  int p1 = 1;
  int p2 = 1;
  while (p1 == p2) {
    p1 = rand.integer(1,3);
    p2 = rand.integer(1,3);
  }
  if (p1 == 1)
    p1Card = "A";
  else if (p1 == 2)
    p1Card = "K";
  else
    p1Card = "Q";
  if (p2 == 1)
    p2Card = "A";
  else if (p2 == 2)
    p2Card = "K";
  else
    p2Card = "Q";
}

std::vector<std::string> KuhnPokerPayoutSet::KuhnPokerGameState::actions() {
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
  if (history.size() == 1 && history[0] == "BET") {
    std::vector<std::string> response;
    response.push_back("FOLD");
    response.push_back("CALL");
    return response;
  }
  std::vector<std::string> response;
  return response;
}

void KuhnPokerPayoutSet::KuhnPokerGameState::makeMove(std::string action) {
  if (action == "BET") {
    pot += 1;
    playerToAct = 1 - playerToAct;
  }
  else if (action == "CHECK") {
    pot += 0;
    playerToAct = 1 - playerToAct;
  }
  else if (action == "FOLD") {
    pot += 0;
    winningPlayer = 1 - playerToAct;
    isTerminalState = true;
  }
  else if (action == "CALL") {
    pot += 1;
    if (p1Card == "A") {
      winningPlayer = 0;
    }
    else if (p2Card == "A") {
      winningPlayer = 1;
    }
    else if (p1Card == "K") {
      winningPlayer = 0;
    }
    else if (p2Card == "K") {
      winningPlayer = 1;
    }
    isTerminalState = true;
  }
}

KuhnPokerInformationSet::KuhnPokerInfomationSet(int player, std::string card, int pot) :
  player(player).
  pot(pot),
  card(card)
{

}

void KuhnPokerInformationSet::makeMove(std::string action, int pot) {
  actions.push_back(action);
  this.pot = pot;
}
