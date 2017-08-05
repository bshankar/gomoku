#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "board.hpp"
#include <algorithm>

class Evaluation {
public:
  double compute(Board board, int turn);
  double center(Board& board, int turn);
  double partials(Board& board, int turn);

private:
  double compareHouses(board_t h1, board_t h2);
  double rowCenter(board_t house, int row);
};

#endif
