#ifndef Card_1484529030900402
#define Card_1484529030900402

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <chrono>
#include <set>
#include <unordered_set>
#include <cstdlib>
#include <tuple>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>

class Card {
 public:
  Card();  
  Card(int rank, int suit);
  std::string ToString();
 private:
  int rank;
  int suit;
  std::string str;
};

#endif
