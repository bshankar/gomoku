#include "search.hpp"
#include <algorithm>
#include <iostream>
 
using std::cout;
using std::vector;
typedef TTable::TTEntry TTEntry;
typedef TTable::TTEntry::Flag Flag;

Search::Search(Board& board) : board(board) {}


bool Search::moveExists(Moves& moves, Move move) {
  for (int i = 0; i < moves.end; ++i)
    if (moves.moveArray[i] == move)
      return true;
  return false; 
}


bool Search::safeInsertMove(Moves& moves, Move move) {
  if (!moveExists(moves, move)) {
    moves.moveArray[moves.end] = move;
    ++moves.end;
    return true;
  }
  return false; 
}


void Search::putSurroundingCells(Moves& moves, House h, House index) {
  House cellsToCheck[8][2] = {{h + 1, index}, {h - 1, index}, {h, index + 1}, {h, index - 1},
                              {h + 1, index + 1}, {h - 1, index - 1},
                              {h + 1, index - 1}, {h - 1, index + 1}};

  for (int i = 0; i < 8; ++i) {
    auto hi = cellsToCheck[i][0];
    auto ii = cellsToCheck[i][1];
    if (hi < 19 && ii < 19 && !board.isFilledAt(hi, ii))
      safeInsertMove(moves, hi*19 + ii);
  }
}


void Search::generateMoves(Moves& moves, bool turn) {
  auto entry = ttable.probe(board.getHash());
  if (entry.flag != Flag::INVALID && entry.key == (board.getHash() >> 48)) {
    auto move = entry.bestMove;
    if (!board.isFilledAt(move/19, move % 19))
      safeInsertMove(moves, move);
  }
  
  for (int i = 0; i < board.getMovesMade().end; ++i) {
    auto move = board.getMovesMade().moveArray[i];
    putSurroundingCells(moves, move/19, move % 19);
  }
}


Eval Search::negamax(int depth, Eval alpha, Eval beta, bool turn) {
  auto alphaOrig = alpha;
  ++nodesVisited;

  // Transposition Table Lookup; node is the lookup key for TTEntry
  TTEntry entry = ttable.probe(board.getHash());
  if (entry.flag != Flag::INVALID && entry.key == (board.getHash() >> 48) &&
      entry.depth >= depth) {
    if (entry.flag == Flag::EXACT)
      return entry.eval;
    else if (entry.flag == Flag::LOWERBOUND)
      alpha = std::max(alpha, entry.eval);
    else if (entry.flag == Flag::UPPERBOUND)
      beta = std::min(beta, entry.eval);
    if (alpha >= beta)
      return entry.eval;
  }
  
  if (!depth || board.winner() != -1)
    return turn ? -board.getEvaluate().getEval()
      : board.getEvaluate().getEval();

  Eval bestValue = -1000000;
  Move bestMove = -1;
  Moves moves;
  generateMoves(moves, turn);
  
  for (int i = 0; i < moves.end; ++i) {
    board.place(turn, moves.moveArray[i]);
    Eval v = -negamax(depth - 1, -beta, -alpha, turn ^ 1);
    board.remove(turn, moves.moveArray[i]);

    if (v > bestValue) {
      bestValue = v;
      bestMove = moves.moveArray[i];
    }
    alpha = std::max(alpha, v);
    if (alpha >= beta)
      break;
  }
  
  // Transposition Table Store; node is the lookup key for TTEntry
  entry.eval = bestValue;
  if (bestValue <= alphaOrig)
    entry.flag = Flag::UPPERBOUND;
  else if (bestValue >= beta)
    entry.flag = Flag::LOWERBOUND;
  else
    entry.flag = Flag::EXACT;

  entry.depth = depth;
  entry.bestMove = bestMove;
  entry.key = board.getHash() >> 48;
  ttable.store(entry, board.getHash());
  return bestValue;
}


emscripten::val Search::getBestMoveInfo(Move bestMove, Eval eval, NodeCount nodes) {
  // In javascript accessible format
  emscripten::val bestMoveInfo = emscripten::val::object();
  bestMoveInfo.set("bestMove", emscripten::val(19*9 + 9));
  bestMoveInfo.set("eval", emscripten::val(0));
  bestMoveInfo.set("nodes", emscripten::val(0));
  return bestMoveInfo;
}


emscripten::val Search::calcBestMove(int depth, bool turn) {
  if (board.isEmpty())
    return getBestMoveInfo(19*9 + 9, 0, 0);

  nodesVisited = 0;
  for (int d = 1; d <= depth; ++d) 
    negamax(d, -1000000, 1000000, turn);

  return getBestMoveInfo(ttable.probe(board.getHash()).bestMove,
                         ttable.probe(board.getHash()).eval,
                         nodesVisited);
}

EMSCRIPTEN_BINDINGS(gomokuSearch) {
  emscripten::class_<Search>("Search")
    .constructor<Board&>()
    .function("calcBestMove", &Search::calcBestMove);
};
