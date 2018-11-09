# Counterfactual Regret Minimization

This is a header-only library to implement Counterfactual Regret Minimization
(CFR) on multiplayer games.

## Prerequisites

To build the program you need CMake (https://cmake.org/download/).

## Getting Started

Counterfactual regret minimization is an algorithm that will iteratively finds
the Nash equilibrium for multiplayer games. The core algorithm is implemented
in CounterfactualRegretMinimizer.h. To find the Nash equilibrium of a new
game, the game's mechanics need to be encoded into the equilibrium solver.
This is done by implementing the interface Game defined in Game.h. Next, we
create an instance of the CounterfactualRegretMinimizer templated on the
given game and call the minimizer's solve method.

Examples on how to call CFR on Kuhn Poker and on Rock-Paper-Scissors are given
in main.cpp.

## Output

The output of this algorithm is a checkpoint file that stores the Nash
equilibrium strategy. At the top of the checkpoint file, there will be
a list of all decisions that each player must make. For each decision, the
minimizer will return a probability distribution over all possible moves.
This distribution is stored as a space separated list, where the n-th value
is the probability of taking the n-th action. For more information, consult the
save method of the CounterfactualRegretMinimizer

## Contact

For more information, send an email to kundanc@mit.edu
