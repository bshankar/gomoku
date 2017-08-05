#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "board.hpp"

class Evaluation {
public:
  double compute(Board board, int turn);
  double center(Board& board, double eval, int turn);
  double partials(Board& board, double eval, int turn);
};

#endif
