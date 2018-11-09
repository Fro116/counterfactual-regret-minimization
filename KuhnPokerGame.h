#pragma once

#include "Game.h"

using KuhnPokerAction = std::string;

/**
 * Class to describle Kuhn poker as an abstract strategy game.
 */
class KuhnPokerGame : public Game<KuhnPokerAction> {
public:
  KuhnPokerGame();

  void beginGame();
  void makeMove(KuhnPokerAction action);

  std::vector<double> payout() const;
  std::string informationSet() const;
  std::shared_ptr<Game<KuhnPokerAction>> deepCopy() const;
  bool isTerminalState() const;
  std::size_t numPlayers() const;
  std::size_t playerToAct() const;
  std::vector<KuhnPokerAction> actions() const;

 private:

  /**
   * Determines which player was dealt the best hand
   *
   * @return the player whose hand is of higher rank
   */
  std::size_t winningPlayer();

  // the cards dealt to each player
  std::vector<std::string> m_cards;

  // the betting history
  std::vector<KuhnPokerAction> m_history;

  // the amount of money that each person has placed in the pot
  std::vector<int> m_pot;

  // the current player
  std::size_t m_player;

  // true if the game has ended
  bool m_end;

  // the winning player
  // this field is only valid if the game has ended
  std::size_t m_winner;
};
