#include "Random.h"

#include <chrono>

Random::RNG Random::engine;

bool Random::seeded = false;

void Random::seed(unsigned int seed) {
  seeded = true;
  engine.seed(seed);
}

Random::RNG& Random::generator(){
  if(!seeded){
    num_type seed = static_cast<num_type>(std::chrono::system_clock::now()
                                          .time_since_epoch().count());
    seeded = true;
    engine.seed(seed);
  }
  return engine;
}

int Random::number(int min, int max){
  std::uniform_int_distribution<> distribution(min,max);
  Random::RNG& algorithm = generator();
  int value = distribution(algorithm);
  return value;
}
