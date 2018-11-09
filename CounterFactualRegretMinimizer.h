#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Game.h"

/**
 * Template header-only library to perform Counterfactual Regret
 * Minimization on multiplayer games.
 *
 * @tparam S the type of actions
 */
template <class S>
class CounterFactualRegretMinimizer {
public:

  /**
   * Initializes the CFR algorithm
   */
  CounterFactualRegretMinimizer(std::shared_ptr<const Game<S>> game);

  /**
   * Runs CFR for an infinite number of iterations. The computed strategy will
   * periodically be saved to an output file.
   *
   * @param outputFile the file to save the current strategy to
   * @param itersPerSave the number of iterations before the strategy
   *     should be checkpointed
   * @param itersPerUpdate the number of iterations before an update message
   *     is printed to the console
   */
  void solve(std::string outputFile, std::size_t itersPerSave,
             std::size_t itersPerUpdate);

  /**
   * Runs CFR for the given number of iterations
   *
   * @param iterations the number of iterations to run
   */
  void train(std::size_t iterations);

  /**
   * Saves the current strategy to the given file
   *
   * @param filename the file to save to
   */
  void save(std::string filename);

  /**
   * Loads the strategy that was saved into the file. Training can be
   * resumed without loss of infomation.
   *
   * @param filename the file to load from
   */
  void load(std::string filename);

private:

  /**
   * Calculates the players current strategy for a given information set
   *
   * @param player the player to move
   * @param id the information set's id
   * @param actions the set of possible actions
   *
   * @return the probability distribution over the player's actions
   */
  std::vector<double> getStrategy(int player, std::string id,
                                  const std::vector<S>& actions) const;

  /**
   * Analyzes one node of the game tree and recursively updates the strategy
   *
   * @param payouts the abstract strategy game to train
   * @param probabilities the i-th element is the probability that player i
   *     causes this information set to occur
   */
  std::vector<double> train(std::shared_ptr<const Game<S>> game,
                            const std::vector<double> probabilities);

  // The abstract strategy game
  const std::shared_ptr<const Game<S>> game;

  // The number of players in the game
  const std::size_t numPlayers;

  // The accumulated regrets for each action. The data is stored in the format
  //     player -> information set id -> list of actions
  std::vector<std::unordered_map<
                std::string, std::vector<double>>> aggregateRegrets;

  // The number of times each action was selected during the course of training
  //     The data is stored in the format player -> information set id ->
  //     list of actions
  std::vector<std::unordered_map<
                std::string, std::vector<double>>> aggregateStrategies;

};


template <class S>
CounterFactualRegretMinimizer<S>::CounterFactualRegretMinimizer(
  std::shared_ptr<const Game<S>> game) :
  game(game),
  numPlayers(game->numPlayers()),
  aggregateRegrets(game->numPlayers(), std::unordered_map<std::string,
                   std::vector<double>>()),
  aggregateStrategies(game->numPlayers(), std::unordered_map<std::string,
                      std::vector<double>>())
{

}


template <class S>
void CounterFactualRegretMinimizer<S>::solve(
  std::string outputFile, std::size_t itersPerSave, std::size_t itersPerUpdate) {

  // initialize training counters
  std::size_t saveCounter = 0;
  std::size_t totalIterations = 0;
  std::cout << "BEGINNING TRAINING" << std::endl;

  while (true) {
    // train the model
    train(itersPerUpdate);

    // print an update to the console
    saveCounter += itersPerUpdate;
    totalIterations += itersPerUpdate;
    std::cout << "COMPLETED ITERATION: " << totalIterations << std::endl;

    // save a checkpoint
    if (saveCounter >= itersPerSave) {
      saveCounter %= itersPerSave;
      std::cout << "SAVING..." << std::endl;
      save(outputFile);
    }
  }
}

template <class S>
void CounterFactualRegretMinimizer<S>::train(std::size_t iterations) {
  // train on the game
  for (std::size_t i = 0; i < iterations; ++i) {
    auto copy = game->deepCopy();
    copy->beginGame();
    const std::vector<double> probabilities(numPlayers, 1.0);
    train(copy, probabilities);
  }
}


template <class S>
std::vector<double> CounterFactualRegretMinimizer<S>::train(
  const std::shared_ptr<const Game<S>> game,
  const std::vector<double> probabilities) {

  // check if the game has ended
  const int player = game->playerToAct();
  if (game->isTerminalState()) {
    return game->payout();
  }

  // create the information set node
  const std::string id = game->informationSet();
  const std::vector<S> actions = game->actions();
  if (aggregateStrategies[player].find(id)
      == aggregateStrategies[player].end()) {

    aggregateStrategies[player][id] = std::vector<double>(actions.size());
    aggregateRegrets[player][id] = std::vector<double>(actions.size());
  }

  // determine the player's strategy
  const std::vector<double> strategy = getStrategy(player, id, actions);
  std::vector<std::vector<double>> actionUtilities;
  std::vector<double> nodeUtilities(numPlayers, 0);

  // Recursively train on each action
  for (std::size_t action = 0; action < actions.size(); ++action) {
    // branch on the player's action
    std::shared_ptr<Game<S>> gameCopy = game->deepCopy();
    gameCopy->makeMove(actions[action]);
    std::vector<double> probabilitiesCopy = probabilities;
    probabilitiesCopy[player] *= strategy[action];

    // update utilities
    actionUtilities.push_back(train(gameCopy, probabilitiesCopy));
    for (std::size_t agent = 0; agent < numPlayers; ++agent) {
      nodeUtilities[agent] += strategy[action]*actionUtilities[action][agent];
    }
  }

  // accumulate counterfactual regret
  for (std::size_t action = 0; action < actions.size(); ++action) {
    // calculate the counterfactual probability
    double counterfactual = 1.0;
    for (int agent = 0; agent < numPlayers; ++agent) {
      if (agent != player) {
        counterfactual *= probabilities[agent];
      }
    }

    // update regrets
    const double regret = actionUtilities[action][player]
      - nodeUtilities[player];
    aggregateRegrets[player][id][action] += counterfactual * regret;
    aggregateStrategies[player][id][action] +=
      counterfactual * strategy[action];
  }

  return nodeUtilities;
}

template <class S>
std::vector<double> CounterFactualRegretMinimizer<S>::getStrategy(
  int player, std::string id, const std::vector<S>& actions) const {

  // load historical data about this information set
  const std::vector<double>& cumulativeRegrets = aggregateRegrets[player]
    .find(id)->second;
  std::vector<double> strategy;
  double normalizingSum = 0;

  // choose actions with probability in proportion to their regret
  for (int action = 0; action < actions.size(); ++action) {
    double regret;
    if (cumulativeRegrets[action]> 0) {
      regret = cumulativeRegrets[action];
    } else {
      regret = 0;
    }
    strategy.push_back(regret);
    normalizingSum += regret;
  }

  // normalize the strategy into a probability distribution
  for (int action = 0; action < actions.size(); ++action) {
    if (normalizingSum > 0) {
      strategy[action] /= normalizingSum;
    } else {
      strategy[action] = 1.0/actions.size();
    }
  }

  return strategy;
}

template <class S>
void CounterFactualRegretMinimizer<S>::save(std::string filename) {
  std::ofstream file(filename);

  // write the final strategy
  file << "PROBABILITIES" << "\n";
  for (int player = 0; player < numPlayers; ++player) {
    file << "PLAYER: " << player << std::endl;
    for (const auto& it : aggregateStrategies[player]) {
      file << it.first;
      const double total = std::accumulate(it.second.begin(), it.second.end(),
                                           0.0);
      for (double value : it.second) {
        file << " " << value / total;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }

  // print out the raw data for the aggregate strategies
  file << "STRATEGIES" << "\n";
  for (int player = 0; player < numPlayers; ++player) {
    file << "PLAYER: " << player << std::endl;
    for (const auto& it : aggregateStrategies[player]) {
      file << it.first << "\t";
      for (double value : it.second) {
        file << " " << value;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }

  // print out the raw data for the aggregate regrets
  file << "REGRETS" << "\n";
  for (int player = 0; player < numPlayers; ++player) {
    file << "PLAYER: " << player << std::endl;
    for (const auto& it : aggregateRegrets[player]) {
      file << it.first << "\t";
      for (double value : it.second) {
        file << " " << value;
      }
      file << "\n";
    }
    file << "END" << "\n";
  }
}

template <class S>
void CounterFactualRegretMinimizer<S>::load(std::string filename) {
  std::ifstream file(filename);
  std::string buffer;

  // read the final strategy
  std::getline(file, buffer);
  if (buffer != "PROBABILITIES") {
    std::cerr << "Could not parse file " << filename << std::endl;
    return;
  }
  for (int player = 0; player < numPlayers; ++player) {
    std::getline(file, buffer); //discard line
    std::getline(file, buffer);
    while (buffer != "END") {
      std::getline(file, buffer);
    }
  }

  // read the raw data for the aggregate strategies
  std::getline(file, buffer);
  if (buffer != "STRATEGIES") {
    std::cerr << "Could not parse file " << filename << std::endl;
    return;
  }
  aggregateStrategies = {};
  for (int player = 0; player < numPlayers; ++player) {
    aggregateStrategies.push_back(
      std::unordered_map<std::string,std::vector<double>>());
    std::getline(file, buffer); //discard line
    std::getline(file, buffer);
    while (buffer != "END") {
      const std::size_t delim_index = buffer.find_first_of('\t');
      const std::string key = buffer.substr(0, delim_index);
      std::stringstream line(buffer.substr(delim_index));
      std::vector<double> value;
      while (line.good()) {
        double entry;
        line >> entry;
        value.push_back(entry);
      }
      aggregateStrategies[player][key] = value;
      std::getline(file, buffer);
    }
  }

  // read the raw data for the aggregate regrets
  std::getline(file, buffer);
  if (buffer != "REGRETS") {
    std::cerr << "Could not parse file " << filename << std::endl;
    return;
  }
  aggregateRegrets = {};
  for (int player = 0; player < numPlayers; ++player) {
    aggregateRegrets.push_back(
      std::unordered_map<std::string,std::vector<double>>());
    std::getline(file, buffer);
    std::getline(file, buffer); //discard line
    while (buffer != "END") {
      const std::size_t delim_index = buffer.find_first_of('\t');
      const std::string key = buffer.substr(0, delim_index);
      std::stringstream line(buffer.substr(delim_index));
      std::vector<double> value;
      while (line.good()) {
        double entry;
        line >> entry;
        value.push_back(entry);
      }
      aggregateRegrets[player][key] = value;
      std::getline(file, buffer);
    }
  }
}
