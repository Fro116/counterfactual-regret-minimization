# Counterfactual Regret Minimization

This is a header-only library to implement Counterfactual Regret Minimization (CFR) on two player games.

## Prerequisites

To build the program you need CMake (https://cmake.org/download/). 

## Getting Started
The core library consists of CounterfactualRegretMinimizer.h and PayoutSet.h. The rest of the files are
examples that implement CFR for various games. Examples on how to call CFR on Rock-Paper-Scissors
and Kuhn Poker are given in main.cpp. 

## Running the Poker Examples
To run CFR on discard poker, a variant of Texas Holdem in which each player case choose to discard a card after the preflop or flop has been dealt, we need several auxillary files describing the bucketing strategy and discard rules we are using. These
files are too large to upload to github. First, download these files from https://www.dropbox.com/s/ds0jmnwxj72etl8/data.tar.gz?dl=0 and unpack them into a directory called data. Then, install the pbots_calc library (https://github.com/mitpokerbots/pbots_calc) and uncomment the relevant files in CMakeLists.txt and main.cpp.


## Contact
For more information, send an email to kundanc@mit.edu
