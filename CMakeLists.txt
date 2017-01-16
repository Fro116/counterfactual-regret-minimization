cmake_minimum_required (VERSION 2.8.11)
project (solver)
add_executable (solver KuhnPokerPayoutSet.cpp RPSPayoutSet.cpp main.cpp Random.cpp DiscardHoldemPayoutSet.cpp DiscardHoldemGameState.cpp DiscardHoldemInformationSet.cpp Card.cpp)
SET(CMAKE_CXX_FLAGS "-std=c++14 -stdlib=libc++ -O3")

find_library(PBOT pbots_calc)
target_link_libraries(solver ${PBOT})
find_library(PEVAL poker-eval)
target_link_libraries(solver ${PEVAL})
