cmake_minimum_required (VERSION 2.8.11)
project (solver)
add_executable (solver KuhnPokerPayoutSet.cpp RPSPayoutSet.cpp main.cpp Random.cpp)
SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -O3")
