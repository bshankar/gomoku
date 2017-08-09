#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>
using std::cout;
using std::vector;


Search::Search(Board& board) : board(board) {}


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


double Search::negamax(int depth, double alpha, double beta, bool turn) {
  if (!depth || board.winner() != -1) {
    return turn ? -board.eval: board.eval;
  }
 
  double bestValue = -1e13;
  row_t moves[361] = {362};
  generateMoves(moves, turn);
  int index = 0;
  while (index < 361 && moves[index] != 362) {
    board.place(moves[index], turn);
    double v = -0.98*negamax(depth - 1, -beta, -alpha, turn^1);
    board.remove(moves[index], turn);

    if (v > bestValue) {
      bestValue = v;
      pv[depth] = moves[index];
    }
    alpha = std::max(v, alpha);
    if (alpha >= beta)
      break;
    ++index;
  }
  return bestValue;
}
