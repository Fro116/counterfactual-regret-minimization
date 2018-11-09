#include "KuhnPokerGame.h"

#include <cassert>

#include "Random.h"

KuhnPokerGame::KuhnPokerGame() {

}

void KuhnPokerGame::beginGame() {
  // deal cards
  std::vector<std::string> cards = {"K", "Q", "J"};
  for (std::size_t player = 0; player < numPlayers(); ++player) {
    const std::size_t sample = static_cast<std::size_t>(
      Random::number(0, cards.size() - 1));
    const std::string card = cards[sample];
    cards.erase(cards.begin() + sample);
    m_cards.push_back(card);
  }

  m_history = {};
  m_pot = {1, 1}; // post antes
  m_player = 0;
  m_winner = 2; // this field is invalid until the game has ended
  m_end = false;
}

std::shared_ptr<Game<KuhnPokerAction>> KuhnPokerGame::deepCopy() const {
  std::shared_ptr<KuhnPokerGame> copy = std::make_shared<KuhnPokerGame>();
  *copy = *this;
  return copy;
}

std::string KuhnPokerGame::informationSet() const {
  // include player information
  std::string id = "[Player: " + std::to_string(m_player)
    + ", Card: " + m_cards[m_player]
    + ", Betting History: ";

  // include betting history
  for (auto it = m_history.begin(); it != m_history.end(); ++it) {
    id += *it;
    if (it + 1 != m_history.end()) {
      id += ", ";
    }
  }

  // include actions
  id += " Actions: ";
  const std::vector<KuhnPokerAction> moves = actions();
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    id += *it;
    if (it + 1 != moves.end()) {
      id += ", ";
    }
  }

  id += "]";
  return id;
}

std::vector<double> KuhnPokerGame::payout() const {
  assert(isTerminalState());

  // detemine the payout
  const double pot = m_pot[0] + m_pot[1];
  if (m_winner == 0) {
    return {pot - m_pot[0], static_cast<double>(-m_pot[1])};
  }
  else if (m_winner == 1) {
    return {static_cast<double>(-m_pot[0]), pot - m_pot[1]};
  }

  // invalid winner
  assert(false);
}

bool KuhnPokerGame::isTerminalState() const {
  return m_end;
}

std::size_t KuhnPokerGame::numPlayers() const {
  return 2;
}

std::size_t KuhnPokerGame::playerToAct() const {
  return m_player;
}

std::vector<KuhnPokerAction> KuhnPokerGame::actions() const {
  if (m_history.size() > 0 && m_history.back() == "BET") {
    return {"CALL", "FOLD"};
  }
  return {"BET", "CHECK"};
}

void KuhnPokerGame::makeMove(KuhnPokerAction action) {
  assert(!isTerminalState());

  if (action == "BET") {
    m_pot[m_player] += 1;
  }
  else if (action ==  "CHECK") {
    if (m_history.size() == 1) {
      assert(m_history[0] == "CHECK");
      m_winner = winningPlayer();
      m_end = true;
    }
  }
  else if (action == "FOLD") {
    m_winner = 1 - m_player;
    m_end = true;
  }
  else if (action == "CALL") {
    m_pot[m_player] += 1;
    m_winner = winningPlayer();
    m_end = true;
  }
  else {
    // invalid action passed
    assert(false);
  }

  m_history.push_back(action);
  m_player = 1 - m_player;
}

std::size_t KuhnPokerGame::winningPlayer() {
  std::size_t winner = 2;
  if (m_cards[0] == "K") {
    winner = 0;
  }
  else if (m_cards[1] == "K") {
    winner = 1;
  }
  else if (m_cards[0] == "Q") {
    winner = 0;
  }
  else if (m_cards[1] == "Q") {
    winner = 1;
  }
  assert(winner != 2);
  return winner;
}
