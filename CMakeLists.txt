cmake_minimum_required (VERSION 3.1.0)
project (solver)
add_executable (solver KuhnPokerPayoutSet.cpp RPSPayoutSet.cpp main.cpp Card.cpp)
SET(CMAKE_CXX_FLAGS "-std=c++14 -stdlib=libc++ -O3")

# to run the poker examples, uncomment the following lines
# target_sources (solver PUBLIC DiscardHoldemPayoutSet.cpp DiscardHoldemGameState.cpp DiscardHoldemInformationSet.cpp DiscardHoldemBucketer.cpp)
# find_library(PBOT pbots_calc)
# target_link_libraries(solver ${PBOT})
