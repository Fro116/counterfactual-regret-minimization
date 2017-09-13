#ifndef DiscardHoldemBucketer_1484690934564688
#define DiscardHoldemBucketer_1484690934564688

#include <string>
#include <vector>
#include <unordered_map>

/** 
 * Class to perform bucketing on hand combinations in
 * discard poker. Bucketing groups similar information
 * sets together to reduce the branching factor of
 * a game tree.
 */

class DiscardHoldemBucketer {
 public:
  static void initBuckets(std::string file);
  static void initCenters(std::string file);
  static void initDiscards(std::string file);
  static int bucket(std::string key);
  static int discard(std::string key);  
  static double distance(std::vector<double> a, std::vector<double> b);
 private:
  static std::unordered_map<std::string, int> bucketmap;
  static std::unordered_map<std::string, int> discardmap;  
  static std::vector<std::vector<double>> centers;
  static std::string riverKeyCache1;
  static std::string riverKeyCache2;  
  static int riverValCache1;
  static int riverValCache2;
  static int riverCache;
};

#endif
