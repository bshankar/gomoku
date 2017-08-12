#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>

using std::cout;
using std::vector;
typedef TTable::TTEntry TTEntry;
typedef TTable::TTEntry::Flag Flag;


Search::Search(Board& board) : board(board) {
  
}


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
  // rows
  for (int i = index + 1; i < index + 5; ++i) {
    if (i > 18)
      break;
    if (!((board.getHouse(0, h) & (1 << i)) ||
          (board.getHouse(1, h) & (1 << i))))
      safeInsertMove(moves, h*19 + i);
  }

  for (int i = index - 1; i > index - 5; --i) {
    if (i < 0)
      break;
    if (!((board.getHouse(0, h) & (1 << i)) ||
          (board.getHouse(1, h) & (1 << i))))
      safeInsertMove(moves, h*19 + i);
  }
  
  // cols
  for (int i = h + 1; i < h + 5; ++i) {
    if (i > 18)
      break;
    if (!((board.getHouse(0, i) & (1 << index)) ||
          (board.getHouse(1, i) & ( 1 << index))))
      safeInsertMove(moves, i*19 + index);
  }

  for (int i = h - 1; i > h - 5; --i) {
    if (i < 0)
      break;
    if (!((board.getHouse(0, i) & (1 << index)) ||
          (board.getHouse(1, i) & (1 << index))))
      safeInsertMove(moves, i*19 + index);
  }
  
  // diag
  for (int i = 1; i < 5; ++i) {
    if (h + i > 18 || index + i > 18)
      break;
    if (!((board.getHouse(0, h + i) & (1 << (index + i))) ||
          (board.getHouse(1, h + i) & (1 << (index + i)))))
      safeInsertMove(moves, (h + i)*19 + (index + i));
  }

  for (int i = 1; i < 5; ++i) {
    if (i > h || i > index)
      break;
    if (!((board.getHouse(0, h - i) & (1 << (index - i))) ||
          (board.getHouse(1, h - i) & (1 << (index - i)))))
      safeInsertMove(moves, (h - i)*19 + (index - i));    
  }
  
  // antidiag
  for (int i = 1; i < 5; ++i) {
    // h + i, index - i 
    if (i > index || h + i > 18)
      break;
    if (!((board.getHouse(0, h + i) & (1 << (index - i))) ||
          (board.getHouse(1, h + i) & (1 << (index - i)))))
      safeInsertMove(moves, (h + i)*19 + (index - i));
  }

  for (int i = 1; i < 5; ++i) {
    // h - i, index + i
    if (i > h || index + i > 18)
      break;
    if (!((board.getHouse(0, h - i) & (1 << (index + i))) ||
          (board.getHouse(1, h - i) & (1 << (index + i)))))
      safeInsertMove(moves, (h - i)*19 + (index + i));
  }
}


void Search::generateMoves(Moves& moves, bool turn) {
  for (int p = 0; p < 2; ++p) 
    for (int h = 0; h < 19; ++h) { 
      auto house = board.getHouse(p, h);

      House index = 0;
      while (house) {
        if (house & 1)
          putSurroundingCells(moves, h, index);
        house >>= 1;
        ++index;
      }
    }
}


Eval Search::negamax(int depth, Eval alpha, Eval beta, bool turn) {
  auto alphaOrig = alpha;

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


Move Search::calcBestMove(int depth, bool turn) {
  if (board.isEmpty())
    return 19*9 + 9;
  negamax(depth, -1000000, 1000000, turn);
  return ttable.probe(board.getHash()).bestMove; 
}
