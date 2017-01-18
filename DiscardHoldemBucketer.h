#ifndef DiscardHoldemBucketer_1484690934564688
#define DiscardHoldemBucketer_1484690934564688

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

class DiscardHoldemBucketer {
 public:
  static void init(std::string file);
  static void initCenters(std::string file);  
  static int bucket(std::string key);
  static double distance(std::vector<double> a, std::vector<double> b);
 private:
  static std::unordered_map<std::string, int> bucketmap;
  static std::vector<std::vector<double>> centers;  
};

#endif
