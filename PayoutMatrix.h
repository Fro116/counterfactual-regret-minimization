#ifndef __PayoutMatrix__1454267750738784__
#define __PayoutMatrix__1454267750738784__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

class PayoutMatrix {
 public:
  virtual std::vector<double> operator() (std::vector<int> indices) = 0;
  virtual int size() = 0;
  virtual int numPlayers() = 0;
 private:

};

#endif
