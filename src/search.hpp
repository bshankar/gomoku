#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"
typedef Board::Move Move;

class Search {
public:
  double negamax(Board& board, int depth, double alpha,
                 double beta, int turn);
  void generateMoves(Move moves[], const Board& board, int turn);
  Move pv[100] = {};
};

#endif
