#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"

class Search {
public:
  Search(Board& board);
  double negamax(int depth, double alpha, double beta, bool turn);
  void generateMoves(row_t moves[], bool turn);
  row_t pv[100] = {};
  
  struct ttEntry {
    row_t key;
    eval_t eval;
    eval_t computedEval;
    uint8_t depth;
    row_t bestMove;
  };

  Board& board;
  std::vector<ttEntry> hashTable;
};

#endif
