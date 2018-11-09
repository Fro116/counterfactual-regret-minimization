#include "RPSGame.h"

#include <cassert>

RPSGame::RPSGame() {

}

void RPSGame::beginGame() {
  m_actions.clear();
  m_player = 0;
}

void RPSGame::makeMove(RPSAction action) {
  assert(!isTerminalState());

  m_actions.push_back(action);
  m_player = 1 - m_player;
}

std::vector<double> RPSGame::payout() const {
  assert(isTerminalState());

  const RPSAction first = m_actions[0];
  const RPSAction second = m_actions[1];

  // determine the winning player
  if ((first == "ROCK" && second == "SCISSORS")
      || (first == "SCISSORS" && second == "PAPER")
      || (first == "PAPER" && second == "ROCK")) {
    // the first player wins
    return {1, -1};
  }
  else if ((second == "ROCK" && first == "SCISSORS")
           || (second == "SCISSORS" && first == "PAPER")
           || (second == "PAPER" && first == "ROCK")) {
    // the second player wins
    return {-1, 1};
  }
  else {
    // the players tied
    assert(first == second);
    return {0, 0};
  }
}

std::shared_ptr<Game<RPSAction>> RPSGame::deepCopy() const {
  std::shared_ptr<RPSGame> copy = std::make_shared<RPSGame>();
  *copy = *this;
  return copy;
}

bool RPSGame::isTerminalState() const {
  return m_actions.size() == 2;
}

std::size_t RPSGame::numPlayers() const {
  return 2;
}

std::size_t RPSGame::playerToAct() const {
  return m_player;
}

std::vector<RPSAction> RPSGame::actions() const {
  return {"ROCK", "PAPER", "SCISSORS"};
}

std::string RPSGame::informationSet() const {
  std::string id = "[Player: " + std::to_string(m_player)
    + ", Actions: ";
  const std::vector<RPSAction> moves = actions();
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    id += *it;
    if (it + 1 != moves.end()) {
      id += ", ";
    }
  }
  id += "]";
  return id;
}
