# Counterfactual Regret Minimization

This is a header-only library to implement Counterfactual Regret Minimization (CFR) on two player games.

## Prerequisites

To build the program you need CMake (https://cmake.org/download/). 

## Getting Started
The core library consists of CounterfactualRegretMinimizer.h and PayoutSet.h. The rest of the files are
examples that implement CFR for various games. Examples on how to call CFR on Rock-Paper-Scissors
and Kuhn Poker are given in main.cpp. To run CFR on discard poker, a variant of Texas Holdem,
we need several auxillary files describing the bucketing strategy and discard rules we are using. These
files are too large to upload to github. For more on generating these files, see 
https://github.com/Fro116/Pokerbots. In addition, the Pbots_calc library needs to be installed
(https://github.com/mitpokerbots/pbots_calc) and linked to the project.
