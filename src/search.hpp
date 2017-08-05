#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.hpp"

class Search {
public:
  Search();
  double negamax(Board board, int depth, double alpha,
              double beta, int turn);
  std::vector<Board> pv;
private:
  std::vector<Board> generateMoves(Board board, int turn);
  std::vector<Board> orderMoves(std::vector<Board> moves, int turn);
  Evaluation eval;
};

#endif
