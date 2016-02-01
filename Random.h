//
//  Random.h
//  Hearts
//
//  Wrapper for a random number generator algorithm
//
//  Created by Kundan Chintamaneni on 2/9/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__Random__
#define __Hearts__Random__

#include <random>
#include <map>
#include <utility>
#include <mutex>

class Random {
public:
    static int integer(int min, int max);
    static double real(int min, int max);
    static void seed(unsigned int seed);
private:
    using RNG = std::mt19937; //algorithm used for random number generation
    using num_type = unsigned int; //numbertype used to store seed

    static RNG& generator();
    
    static RNG engine;
    static bool seeded;
    
    static std::mutex RNGmutex;
};

#endif /* defined(__Hearts__Random__) */
