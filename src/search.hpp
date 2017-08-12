#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"
#include "evaluate.hpp"
#include "tt.hpp"

typedef uint8_t Depth;
typedef Board::Moves Moves;


class Search {
public:
  Search(Board& board);
  Move calcBestMove(int depth, bool turn);

private:
  Board& board;
  TTable ttable;
  void generateMoves(Moves& moves, bool turn);
  void putSurroundingCells(Moves& moves, House h, House index);
  bool moveExists(Moves& moves, Move move);
  bool safeInsertMove(Moves& moves, Move move);
  Eval negamax(int depth, Eval alpha, Eval beta, bool turn);

};

#endif
