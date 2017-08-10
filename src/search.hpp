#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"
typedef uint8_t Depth;

class Search {
public:
  Search(Board& board);
  Eval negamax(int depth, Eval alpha, Eval beta, bool turn);
  void generateMoves(Move moves[], bool turn);
  
  struct TTEntry {
    Key key;                  // 2 bytes
    Eval eval;                // 4 bytes        
    Depth depth;              // 1 byte
    Move bestMove;            // 2 bytes
    
    enum Flag : Move {        // 1 byte
      UPPERBOUND,
      LOWERBOUND,
      EXACT,
      INVALID
    };
    Flag flag = Flag::INVALID;
  };

  Board& board;
  std::vector<TTEntry> hashTable;
};

#endif
