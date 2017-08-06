#include "search.hpp"
#include <algorithm>
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
  // order moves by center influence
  std::sort(moves.begin(), moves.end(), [](Move m1, Move m2) {
      return 18 - abs(m1.row - 19/2) - abs(m1.col - 19/2) >
        18 - abs(m2.row - 19/2) - abs(m2.col - 19/2);
    });
  return moves;
}

double Search::negamax(Board& board, int depth,
                       double alpha, double beta, int turn) {
  if (!depth || board.winner() != -1) {
    return turn ? -board.eval: board.eval;
  }

  auto nextMoves = generateMoves(board, turn);
  double bestValue = -1e200;
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

