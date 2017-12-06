#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include "board.hpp"
#include "evaluate.hpp"
#include "tt.hpp"
#include <emscripten/bind.h>

typedef uint8_t Depth;
typedef Board::Moves Moves;
typedef House NodeCount;

class Search {
public:
  Search();
  emscripten::val calcBestMove(int depth, bool turn);
  emscripten::val getBestMoveInfo(Move bestMove, Eval eval, NodeCount nodes);
private:
  Board& board;
  TTable ttable;
  void generateMoves(Moves& moves, bool turn);
  void orderMoves(Moves& moves, bool turn);
  void putSurroundingCells(Moves& moves, House h, House index);
  bool moveExists(Moves& moves, Move move);
  bool safeInsertMove(Moves& moves, Move move);
  Eval negamax(int depth, Eval alpha, Eval beta, bool turn);
  NodeCount nodesVisited = 0;
};

#endif
