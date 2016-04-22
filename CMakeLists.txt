cmake_minimum_required (VERSION 2.8.11)
project (solver)
add_executable (solver KuhnPokerPayoutSet.cpp RegretMinimizer.cpp PayoutMatrix.cpp TicTacToePayoutMatrix.cpp PayoutSet.cpp main.cpp Random.cpp)
SET(CMAKE_CXX_FLAGS "-std=c++11")
