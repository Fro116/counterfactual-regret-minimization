#pragma once

#include "Game.h"

using RPSAction = std::string;

/**
 * Class to represent Rock-Paper-Scissors as an abstract strategy game.
 */
class RPSGame : public Game<RPSAction> {
public:
  RPSGame();

  void beginGame();
  void makeMove(RPSAction action);

  std::vector<double> payout() const;
  std::shared_ptr<Game<RPSAction>> deepCopy() const;
  std::string informationSet() const;
  bool isTerminalState() const;
  std::size_t numPlayers() const;
  std::size_t playerToAct() const;
  std::vector<RPSAction> actions() const;

private:
  // the action that each player chose
  std::vector<RPSAction> m_actions;

  // the current player
  std::size_t m_player;
};
