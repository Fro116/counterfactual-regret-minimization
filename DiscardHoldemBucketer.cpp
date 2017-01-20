#include "DiscardHoldemBucketer.h"

#include <boost/algorithm/string.hpp>
#include <pbots_calc.h>

int DiscardHoldemBucketer::bucket(std::string key) {
  if (key.size() == 10) {
    return bucketmap[key];
  } else {
    // std::vector<double> point;
    // std::string hand = key.substr(0,4);
    // std::string board = key.substr(4);
    // std::string dead = "";
    // Results* res = alloc_results();    
    // calc(const_cast<char*>((hand+":"+"23s,24s,25s,26s,27s,34s,35s,36s,37s,45s,46s,32o,43o,42o,54o,53o,52o,65o,64o,63o,62o,74o,73o,72o,83o,82o").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"28s,29s,2Ts,38s,39s,47s,48s,49s,75o,85o,84o,95o,94o,93o,92o,T5o,T4o,T3o,T2o,J3o,J2o").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"3Ts,4Ts,56s,57s,58s,59s,5Ts,67s,68s,69s,6Ts,78s,79s,89s,67o,68o,69o,6To,78o,79o,7To,89o,8To").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"22,J2s,J3s,J4s,J5s,J6s,Q2s,Q3s,Q4s,Q5s,K2s,J4o,J5o,J6o,J7o,Q2o,Q3o,Q4o,Q5o,Q6o,Q7o,K2o,K3o,K4o").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"6Qs,7Ts,7Js,7Qs,8Ts,8Js,8Qs,8Ts,9Ts,9Js,9Qs,TJs,T9o,J8o,J9o,JTo,Q8o,Q9o,QTo,QJo").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"33,44,55,K3s,K4s,K5s,K6s,K7s,K8s,A2s,A3s,A4s,A5s,A6s,K5o,K6o,K7o,K8o,K9o,A2o,A3o,A4o,A5o,A6o,A7o,A8o").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"66,77,QTs,QJs,K9s,KTs,KJs,KQs,A7s,A8s,A9s,ATs,AJs,AQs,AKs,KTo,KJo,KQo,A9o,ATo,AJo,AQo,AKo").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);
    // point.push_back(*(res->ev));
    // calc(const_cast<char*>((hand+":"+"88,99,TT,JJ,QQ,KK,AA").c_str()), const_cast<char*>(board.c_str()), const_cast<char*>(dead.c_str()), 1000000, res);    
    // point.push_back(*(res->ev));
    // free_results(res);           
    
    // double minDist = std::numeric_limits<double>::max();
    // int bucket = 0;
    // for (std::size_t i = 0; i < centers.size(); ++i) {
    //   double dist = distance(point, centers[i]);
    //   if (dist < minDist) {
    // 	minDist = dist;
    // 	bucket = i;
    //   }
    // }
    // return bucket;
  }
  return 0;  
}

void DiscardHoldemBucketer::init(std::string filename) {
  std::string line;
  std::ifstream file(filename);
  while (std::getline(file, line)) {
    auto loc = line.find_first_of(' ');
    std::string key = line.substr(0, loc);
    int bucket = std::stoi(line.substr(loc+1));
    bucketmap[key] = bucket;
  }
}

void DiscardHoldemBucketer::initCenters(std::string filename) {
  std::string line;
  std::ifstream file(filename);
  while (std::getline(file, line)) {
    std::vector<double> center;    
    std::vector<std::string> words;
    boost::split(words, line, boost::is_any_of("\t "));
    for (std::string word : words) {
      center.push_back(std::stod(word));
    }
    centers.push_back(center);
  }  
}

double DiscardHoldemBucketer::distance(std::vector<double> a, std::vector<double> b) {
  double dist = 0;
  for (std::size_t i = 0; i < a.size(); ++i) {
    dist += (a[i] - b[i])*(a[i] - b[i]);
  }
  return dist;
}

std::unordered_map<std::string, int> DiscardHoldemBucketer::bucketmap;
std::vector<std::vector<double>> DiscardHoldemBucketer::centers;
