#include "search.hpp"
using std::vector;


vector<Move> Search::generateMoves(const Board& board, int turn) {
  vector<Move> moves;
  for (int r = 0; r < 19; ++r) {
    auto brd = ~(board.houses[0][r] | board.houses[1][r]);
    brd &= 0x7ffff;
    auto c = 0;
    while (brd) {
      c += __builtin_ffs(brd) - 1;
      brd >>= __builtin_ffs(brd) - 1;
      Move move = {r, c, turn};
      moves.push_back(move);
      brd--;
    }
  }
  return moves;
}

double Search::negamax(Board& board, int depth,
                       double alpha, double beta, int turn) {
  if (!depth || board.winner() != -1) {
    return turn ? -board.eval: board.eval;
  }

  auto nextMoves = generateMoves(board, turn);
  double bestValue = -1e100;
  Move bestMove;
  for (auto move: nextMoves) {
    board.place(move);
    double v = -negamax(board, depth - 1, -beta, -alpha, turn^1);
    board.remove(move);

    if (v > bestValue) {
      bestValue = v;
      bestMove = move;
    }
    alpha = std::max(v, alpha);
    if (alpha >= beta)
      break;
  }
  pv[depth] = bestMove;
  return bestValue;
}

