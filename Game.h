#pragma once

#include <memory>
#include <string>
#include <vector>

/**
 * Interface for abstract strategy games.
 *
 * @tparam S the type of actions
 */
template <class S>
class Game {
public:

  /**
   * Starts a new game
   */
  virtual void beginGame() = 0;

  /**
   * The player to act makes the given move
   *
   * @param move the move to make
   */
  virtual void makeMove(S move) = 0;

  /**
   * Computes the payouts for a game. Can only be called if the game
   *     has finished.
   *
   * @returns a vector whose i-th value is the payout to the i-th player
   */
  virtual std::vector<double> payout() const = 0;

  /**
   * Returns a deep copy of this object
   *
   * @returns a deep copy
   */
  virtual std::shared_ptr<Game<S>> deepCopy() const = 0;

  /**
   * Returns a unique id of current player's information set.
   *     An information set consists of all the information that player knows
   *     at a given point. Every action node in the game tree corresponds
   *     to exactly one information set.
   *
   * The unique id is a string representation of the information set. The string
   *     representations must be in one-to-one correspondence with the
   *     information sets.
   *
   * The unique id must not contain any '\n' or '\t' characters
   *
   * @returns the current player's information set
   */
  virtual std::string informationSet() const = 0;

  /**
   * Checks if the game has ended
   *
   * @returns true if the game has ended
   */
  virtual bool isTerminalState() const = 0;

  /**
   * Returns the number of players in the game.
   *
   * @returns the number of players in the game
   */
  virtual std::size_t numPlayers() const = 0;

  /**
   * Determines which player's turn it is. Players are numbered
   *     using zero based indexing.
   *
   * @returns the player whose turn it is
   */
  virtual std::size_t playerToAct() const = 0;

  /**
   * Computes the possible actions that the current player can make.
   *     The order of the actions returned must be deteministic
   *
   * @returns a vector of possible actions that the player can make
   */
  virtual std::vector<S> actions() const = 0;
};
