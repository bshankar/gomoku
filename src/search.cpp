#include "search.hpp"
#include "data.hpp"
#include <algorithm>
#include <iostream>
using std::cout;
using std::vector;


void Search::generateMoves(Move moves[], const Board& board, int turn) {
  int index = 0;
  for (int i = 0; i < 361; ++i) {
    auto r = bestCells[i][0],
      c = bestCells[i][1];
    if (board.houses[0][r] & (1 << c) || board.houses[1][r] & (1 << c))
      continue;
    moves[index] = {r, c, turn};
    ++index;
  }
}


double Search::negamax(Board& board, int depth,
                       double alpha, double beta, int turn) {
  if (!depth || board.winner() != -1) {
    return turn ? -board.eval: board.eval;
  }
 
  double bestValue = -1e13;
  Move moves[361] = {};
  generateMoves(moves, board, turn);
  int index = 0;
  while (index < 361 && moves[index].row != -1) {
    board.place(moves[index]);
    double v = -0.98*negamax(board, depth - 1, -beta, -alpha, turn^1);
    board.remove(moves[index]);

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
