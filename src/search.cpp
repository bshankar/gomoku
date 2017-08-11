#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>
using std::cout;
using std::vector;
typedef Search::TTEntry::Flag Flag;
typedef Search::Moves Moves;

Search::Search(Board& board) : board(board) {
  hashTable.resize(1 << 24);
}


void Search::putSurroundingCells(Moves moves, House h, House index) {
  // rows
  
  // cols

  // diag

  // antidiag
}


void Search::generateMoves(Moves moves, bool turn) {
  for (int p = 0; p < 2; ++p) 
    for (int h = 0; h < 19; ++h) { 
      auto house = board.houses[p][h];

      House index = 0;
      while (house) {
        if (h & 1)
          putSurroundingCells(moves, h, index);
        house >>= 1;
        ++index;
      }
    }
}


Eval Search::negamax(int depth, Eval alpha, Eval beta, bool turn) {
  auto alphaOrig = alpha;

  // Transposition Table Lookup; node is the lookup key for TTEntry
  TTEntry entry = hashTable[board.hash & 0xffffff];
  if (entry.flag != Flag::INVALID && entry.key == (board.hash >> 48) &&
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
    return turn ? -board.eval : board.eval;

  Eval bestValue = -1000000;
  Move bestMove = -1;
  Moves moves;
  
  generateMoves(moves, turn);

  for (int i = 0; i < moves.end; ++i) {
    board.place(moves.moveArray[i], turn);
    Eval v = -negamax(depth - 1, -beta, -alpha, turn ^ 1);
    board.remove(moves.moveArray[i], turn);

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
  entry.key = board.hash >> 48;
  hashTable[board.hash & 0xffffff] = entry;
  return bestValue;
}

