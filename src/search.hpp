#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "evaluation.hpp"

class Search {
public:
  Search();
  double negamax(Board& board, int depth, double alpha,
              double beta, int turn);
  Board pv[100];
private:
  std::vector<Board> generateMoves(const Board& board, int turn);
  Evaluation eval;
};

#endif
