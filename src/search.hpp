#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"
typedef uint8_t Depth;
typedef Board::Moves Moves;

class Search {
public:
  Search(Board& board);
  Eval negamax(int depth, Eval alpha, Eval beta, bool turn);
  Move calcBestMove(int depth, bool turn);
  
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

  void generateMoves(Moves& moves, bool turn);
  void putSurroundingCells(Moves& moves, House h, House index);
  bool moveExists(Moves& moves, Move move);
  bool safeInsertMove(Moves& moves, Move move);
};

#endif
