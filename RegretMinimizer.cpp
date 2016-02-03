#include "RegretMinimizer.h"
#include <limits>

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

std::vector<double> RegretMinimizer::exploitability() {
  std::vector<double> exploits;
  for (int player = 0; player < numPlayers; ++player) {
    double totalStrategy = 0;
    for (double strategy : strategies[player]) {
      totalStrategy += strategy;
    }
    double maxExploit = std::numeric_limits<double>::lowest();
    std::vector<int> preactions;
    for (int i = 0; i < numPlayers - 1; ++i) {
      preactions.push_back(0);
    }
    preactions[0] = -1;
    while (permute(preactions)) {
      double exploit = 0;
      for (int i = 0; i < strategies[player].size(); ++i) {
	std::vector<int> actions = preactions;
	actions.insert(actions.begin()+player, i);
	double prob = strategies[player][i] / totalStrategy;
	double value = -((*payout)(actions))[player];
	exploit += prob*value;
      }
      if (exploit > maxExploit) {
	maxExploit = exploit;
      }
    }
    exploits.push_back(maxExploit);
  }
  return exploits;
}

bool RegretMinimizer::permute(std::vector<int>& actions) {
  actions[0]++;
  for (int i = 0; i < actions.size(); ++i) {
    if (actions[i] == payout->size()) {
      if (i == actions.size() - 1) {
	return false;
      }
      actions[i] = 0;
      actions[i+1]++;
    }
  }
  return true;
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
	regrets[player][index] += regret;
	if (regrets[player][index] < 0) {
	  regrets[player][index] = 0;
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
    std::vector<double> exploits = exploitability();
    std::cout << "Exploitability:";
    for (int index = 0; index < exploits.size(); ++index) {
      std::cout << " " << exploits[index];
    }
    std::cout << "\n";
    std::cout << std::endl;
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
