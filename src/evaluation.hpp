#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "board.hpp"

class Evaluation {
public:
  Evaluation(Board& board);
  int compute();
};

#endif
