#pragma once

#include <random>

class Random {
public:
  // returns a random integer in the range [min, max]
  static int number(int min, int max);
  static void seed(unsigned int seed);
private:
  using RNG = std::mt19937; //algorithm used for random number generation
  using num_type = unsigned int; //numbertype used to store seed

  static RNG& generator();

  static RNG engine;
  static bool seeded;
};
