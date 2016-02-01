#include "RegretMinimizer.h"

RegretMinimizer::RegretMinimizer(std::shared_ptr<PayoutMatrix> matrix) : 
  payout(matrix), 
  numPlayers(matrix->numPlayers()),
  regrets(),
  random()
{
  for (int i = 0; i < numPlayers; ++i) {
    std::vector<double> regret;
    std::vector<double> strategy;
    for (int j = 0; j < matrix->size(); ++j) {
      regret.push_back(0);
      strategy.push_back(0);
    }
    regrets.push_back(regret);
    strategies.push_back(strategy);
  }
}

void RegretMinimizer::train(int iterations) {
    std::cout << "Starting training:" << "\n";
    std::cout << "Total Regrets: " << "\n";
    for (int player = 0; player < numPlayers; ++player) {
      std::cout << "Player " << player << ":";
      for (int index = 0; index < regrets[player].size(); ++index) {
	std::cout << " " << regrets[player][index];
      }
      std::cout << "\n";
    }
    std::cout << "Strategies: " << "\n";
    for (int player = 0; player < numPlayers; ++player) {
      std::cout << "Player " << player << ":";
      for (int index = 0; index < strategies[player].size(); ++index) {
	std::cout << " " << strategies[player][index];
      }
      std::cout << "\n";
    }
    std::cout << std::endl;

  for (int iteration = 0; iteration < iterations; ++iteration) {
    std::cout << "Iteration: " << iteration << "\n";
    std::vector<int> actions;
    for (int player = 0; player < numPlayers; ++player) {
      std::vector<double> strategy = strategyProfile(player);
      for (int index = 0; index < strategy.size(); ++index) {
	strategies[player][index] += strategy[index];
      }
      actions.push_back(chooseMove(strategy));
    }
    std::cout << "Strategies: " << "\n";
    for (int player = 0; player < numPlayers; ++player) {
      std::cout << "Player " << player << ":";
      double sum = 0;
      for (double strategy : strategies[player]) {
	sum += strategy;
      }
      for (int index = 0; index < strategies[player].size(); ++index) {
	std::cout << " " << strategies[player][index] / sum;
      }
      std::cout << "\n";
    }
    std::cout << "Actions: ";
    for (int index = 0; index < actions.size(); ++index) {
      std::cout << " " << actions[index];
    }
    std::cout << "\n";
      std::cout << "Immediate Regrets:" << "\n";
    for (int player = 0; player < numPlayers; ++player) {
      std::cout << "Player " << player << ":";
      for (int index = 0; index < strategies[player].size(); ++index) {
	std::vector<int> alternative = actions;
	alternative[player] = index;
	double regret =  ((*payout)(alternative))[player] - ((*payout)(actions))[player];
	std::cout << " " << regret;
	if (regret > 0) {
	  regrets[player][index] += regret;
	}
      }
      std::cout << "\n";
    }
    std::cout << "Total Regrets: " << "\n";
    for (int player = 0; player < numPlayers; ++player) {
      std::cout << "Player " << player << ":";
      for (int index = 0; index < regrets[player].size(); ++index) {
	std::cout << " " << regrets[player][index];
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
}

int RegretMinimizer::chooseMove(std::vector<double> strategy) {
  double value = random.real(0, 1);
  double prob = 0;
  for (int i = 0; i < strategy.size(); ++i) {
    prob += strategy[i];
    if (value < prob) {
      return i;
    }
  }
  return strategy.size() - 1;
}

std::vector<double> RegretMinimizer::strategyProfile(int player) {
  int totalRegrets = 0;
  for (int regret : regrets[player]) {
    totalRegrets += regret;
  }
  std::vector<double> strategy;
  if (totalRegrets == 0) {
    for (int i = 0; i < regrets[player].size(); ++i) {
      strategy.push_back(1.0/regrets[player].size());
    }
  }
  else {
    for (int i = 0; i < regrets[player].size(); ++i) {
      strategy.push_back(regrets[player][i]/totalRegrets);
    }
  }
  return strategy;
}
