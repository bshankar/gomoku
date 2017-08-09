#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>
using std::cout;
using std::vector;
typedef Search::ttEntry::Flag Flag;


Search::Search(Board& board) : board(board) {
  hashTable.resize(1 << 24);
}


void Search::generateMoves(row_t moves[], bool turn) {
  int index = 0;
  for (int i = 0; i < 361; ++i) {
    auto r = bestCells[i][0],
      c = bestCells[i][1];
    if (board.houses[0][r] & (1 << c) || board.houses[1][r] & (1 << c))
      continue;
    moves[index] = r*19 + c;
    ++index;
  }
}

eval_t Search::negamax(int depth, eval_t alpha, eval_t beta, bool turn) {
  auto alphaOrig = alpha;

  // Transposition Table Lookup; node is the lookup key for ttEntry
  ttEntry entry = tableProbe();
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

  eval_t bestValue = -32768;
  row_t moves[361] = {362};
  generateMoves(moves, turn);
  auto index = 0;
  while (index < 361 && moves[index] != 362) {
    board.place(moves[index], turn);
    eval_t v = -negamax(depth - 1, -beta, -alpha, turn ^ 1);
    board.remove(moves[index], turn);

    bestValue = std::max(bestValue, v);
    alpha = std::max(alpha, v);
    if (alpha >= beta)
      break;
  }
  
  // Transposition Table Store; node is the lookup key for ttEntry
  entry.eval = bestValue;
  if (bestValue <= alphaOrig)
    entry.flag = Flag::UPPERBOUND;
  else if (bestValue >= beta)
    entry.flag = Flag::LOWERBOUND;
  else
    entry.flag = Flag::EXACT;

  entry.depth = depth;
  tableStore();

  return bestValue;
}


Search::ttEntry Search::tableProbe() {
  return hashTable[board.hash & 0xffffff];
}


void Search::tableStore() {
  auto entry = tableProbe();
  entry.key = board.hash >> 48;
}
