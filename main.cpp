#include "CounterFactualRegretMinimizer.h"
#include "KuhnPokerGame.h"
#include "RPSGame.h"

/**
 * Example usage of performing CFR. The following code solves the game of
 *     Kuhn poker (https://en.wikipedia.org/wiki/Kuhn_poker) and
 *     periodically saves the strategy in the output file
 *
 * @param outputFile the file to store checkpoints in
 */
static void solveKuhnPoker(std::string outputFile) {
  auto game = std::make_shared<KuhnPokerGame>();
  CounterFactualRegretMinimizer<KuhnPokerAction> trainer(game);
  trainer.solve(outputFile, 100000, 10000);
}

/**
 * Performs CFR on Rock-Paper-Scissors. The strategy will be periodically
 *     stored in the output file
 *
 * @param outputFile the file to store checkpoints in
 */
static void solveRPS(std::string outputFile) {
  const auto game = std::make_shared<RPSGame>();
  CounterFactualRegretMinimizer<RPSAction> trainer(game);
  trainer.solve(outputFile, 100000, 10000);
}

int main(int argc, char *argv[]) {
  solveRPS("rps_strategy.txt");
}
