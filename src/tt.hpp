#ifndef TT_HPP
#define TT_HPP

#include "board.hpp"
#include "evaluate.hpp"
#include <vector>
typedef uint8_t Depth;

class TTable {
public:
  TTable();
  
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
  
  TTEntry probe(Hash hash);
  void store(TTEntry entry, Hash hash);
  
private: 
  std::vector<TTEntry> hashTable;
};

#endif
