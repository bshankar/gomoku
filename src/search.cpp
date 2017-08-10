#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>
using std::cout;
using std::vector;
typedef Search::TTEntry::Flag Flag;


Search::Search(Board& board) : board(board) {
  hashTable.resize(1 << 24);
}


void Search::generateMoves(Move moves[], bool turn) {
  int index = 0;
  for (int i = 0; i < 361; ++i) {
    auto r = bestMoves[i]/19,
      c = bestMoves[i] % 19;
    if (board.houses[0][r] & (1 << c) || board.houses[1][r] & (1 << c))
      continue;
    moves[index] = r*19 + c;
    ++index;
  }
}

Eval Search::negamax(int depth, Eval alpha, Eval beta, bool turn) {
  auto alphaOrig = alpha;

  // Transposition Table Lookup; node is the lookup key for TTEntry
  TTEntry entry = hashTable[board.hash & 0xffffff];
  if (entry.valid && entry.depth >= depth) {
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
  Move moves[361] = {362};
  generateMoves(moves, turn);
  auto index = 0;
  while (index < 361 && moves[index] != 362) {
    board.place(moves[index], turn);
    Eval v = -negamax(depth - 1, -beta, -alpha, turn ^ 1);
    board.remove(moves[index], turn);

    if (v > bestValue) {
      bestValue = v;
      bestMove = moves[index];
    }

    alpha = std::max(alpha, v);
    if (alpha >= beta)
      break;
    ++index; 
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
  entry.valid = true;
  hashTable[board.hash & 0xffffff] = entry;
  return bestValue;
}

