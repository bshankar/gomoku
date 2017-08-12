#include "evaluate.hpp"


Eval Evaluate::getEval() {
  return eval; 
}


void Evaluate::addToEval(Eval value) {
  eval += value;
}


Eval Evaluate::getPartialEval(Move h) {
  return partialEvals[h];
}


void Evaluate::setPartialEval(Move h, Eval value) {
  partialEvals[h] = value;
}


Eval Evaluate::compareHouses(House h1, House h2) {
  House bitmask = 0b11111;
  double eval = 0;

  double patternScores[] = {0, 1, 21, 337, 4045};
  
  while (h1 | h2) {
    auto res = h1 & bitmask,
      resOpp = h2 & bitmask;

    if (res != 0 && resOpp == 0) {
      eval += patternScores[__builtin_popcount(res)];
    }
    
    else if (res == 0 && resOpp != 0) {
      eval -= patternScores[__builtin_popcount(resOpp)];
    }

    h1 >>= 1;
    h2 >>= 1;
  }
  return eval;
}
