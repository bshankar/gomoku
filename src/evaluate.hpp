#ifndef EVALUATE_H
#define EVALUATE_H

#include <cinttypes>
typedef uint16_t Move;
typedef int32_t Eval;
typedef uint32_t House;


class Evaluate {
public:
  void updatePartials(Move move, bool player);
  void updateEval(Move move, bool player);
  Eval getEval();
  void addToEval(Eval value);
  Eval getPartialEval(Move h);
  void setPartialEval(Move h, Eval value);
  Eval compareHouses(House h1, House h2);

private:
  // evaluation related stuff
  Eval eval = 0;
  Eval partialEvals[112] = {};
};

#endif
