#ifndef Card_1484529030900402
#define Card_1484529030900402

#include <string>

/**
 * Class to represent a playing card
 */
class Card {
 public:
  Card();  
  Card(int rank, int suit);
  Card(std::string name);  
  std::string toString();
  int rank();
  int suit();
  bool operator==(const Card &c1) const;
 private:
  int r;
  int s;  
  static std::string ranks[];
  static std::string suits[];  
};

#endif
