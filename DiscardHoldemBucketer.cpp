#include "DiscardHoldemBucketer.h"

int DiscardHoldemBucketer::bucket(std::string key) {
  return bucketmap[key];
}

void DiscardHoldemBucketer::init(std::string filename) {
  std::string line;
  std::ifstream file(filename);
  while (std::getline(file, line)) {
    auto loc = line.find_first_of(' ');
    std::string key = line.substr(0, loc);
    int bucket = std::atoi(line.substr(loc+1).c_str());
    bucketmap[key] = bucket;
  }
}

std::unordered_map<std::string, int> DiscardHoldemBucketer::bucketmap;
