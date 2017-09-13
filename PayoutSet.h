#ifndef __PayoutSet__1454376039745180__
#define __PayoutSet__1454376039745180__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>

/** 
 * Interface for abstract strategy games.
 *
 * S is the type of actions. T is the type of informationSets. 
 */
template <class S, class T>
class PayoutSet {
 public:
  /**
   * Computes the payouts for a game. Can only be called if the game
   * has finished.
   *
   * @returns a vector whose i-th value is the payout to the i-th player
   */
  virtual std::vector<double> payout() = 0;

  /**
   * Starts a new game
   */
  virtual void beginGame() = 0;

  /**
   * Modifies the game state to account for the given move
   */
  virtual void makeMove(S action) = 0;

  /**
   * Returns a deep copy of this object
   * 
   * @returns a deep copy
   */
  virtual std::shared_ptr<PayoutSet<S, T>> deepCopy() = 0;

  /**
   * Returns the information sets sets for each player. An information
   * set consists of all the information that player knows at a 
   * given point. Every action node in the game tree corresponds
   * to a unique information set.
   * 
   * @returns a vector of information sets whose i-th value is
   *   the information set for player i 
   */
  virtual std::vector<T> infoSets() = 0;

  /**
   * Checks if the game has ended
   * 
   * @returns true if the game has ended
   */
  virtual bool isTerminalState() = 0;

  /**
   * Returns the number of players in the game.
   * 
   * @returns the number of players in the game
   */
  virtual int numPlayers() = 0;

  /**
   * Determines which player's turn it is. Players are numbered
   * using zero based indexing.
   *
   * @returns the player whose turn it is
   */
  virtual int playerToAct() = 0;

  /**
   * Computes the possible actions that the current player can make
   *
   * @returns a vector of possible actions that the player can make
   */
  virtual std::vector<S> actions() = 0;

  /**
   * Returns a string representation of the information set. Used
   * for saving a strategy to a file and for hashing information 
   * sets..
   */
  virtual std::string uniqueIdentifier(T set) = 0;

};

#endif
