//
//  Random.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/9/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "Random.h"

#include <random>
#include <chrono>
#include <iostream>

Random::RNG Random::engine;

bool Random::seeded = false;

std::mutex Random::RNGmutex;

void Random::seed(unsigned int seed) {
    seeded = true;
    engine.seed(seed);
}

Random::RNG& Random::generator(){
    if(!seeded){
        num_type seed = static_cast<num_type>(std::chrono::system_clock::now().time_since_epoch().count());
        seeded = true;
        engine.seed(seed);
    }
    return engine;
}

int Random::integer(int min, int max){
    std::lock_guard<std::mutex> lock(RNGmutex);
    std::uniform_int_distribution<> distribution(min,max);
    Random::RNG& algorithm = generator();
    int value = distribution(algorithm);
    return value;
}

double Random::real(int min, int max){
    std::lock_guard<std::mutex> lock(RNGmutex);
    std::uniform_real_distribution<> distribution(min,max);
    Random::RNG& algorithm = generator();
    double value = distribution(algorithm);
    return value;
}
