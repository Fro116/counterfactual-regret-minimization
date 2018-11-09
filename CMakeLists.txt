cmake_minimum_required (VERSION 3.1.0)
project (solver)
add_executable (solver KuhnPokerGame.cpp RPSGame.cpp Random.cpp main.cpp)
SET(CMAKE_CXX_FLAGS "-std=c++14 -O3")
