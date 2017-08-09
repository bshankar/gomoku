#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"

class Search {
public:
  Search(Board& board);
  eval_t negamax(int depth, eval_t alpha, eval_t beta, bool turn);
  void generateMoves(row_t moves[], bool turn);
  row_t pv[100] = {};
  
  struct ttEntry {
    row_t key;                  // 2 bytes
    eval_t eval;                // 2 bytes        
    uint8_t depth;              // 1 byte
    row_t bestMove;             // 2 bytes
    
    enum Flag : uint8_t {       // 1 byte
      UPPERBOUND,
      LOWERBOUND,
      EXACT
    };

    Flag flag;
    bool valid = false;
  };

  Board& board;
  std::vector<ttEntry> hashTable;
  ttEntry tableProbe();
  void tableStore();
};

#endif
