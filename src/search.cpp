#include "search.hpp"
#include <algorithm>
using std::vector;

Search::Search() {
  pv.resize(100);
}

vector<Board> Search::generateMoves(Board board, int turn) {
  vector<Board> moves;
  for (int r = 0; r < 19; ++r)
    for (int c = 0; c < 19; ++c)
      if (!board.rows[0][r][c] && !board.rows[1][r][c]) {
        Board tmp = board;
        tmp.place(r, c, turn);
        moves.push_back(tmp);
      } 
  return moves;
}

vector<Board> Search::orderMoves(vector<Board> moves, int turn) {
  std::sort(moves.begin(), moves.end(), [&](Board b1, Board b2) {
      return eval.compute(b1, turn) > eval.compute(b2, turn);
    });
  return moves;
}

double Search::negamax(Board board, int depth,
                       double alpha, double beta, int turn) {

  if (!depth || board.winner() != -1) {
    Evaluation eval;
    pv[depth] = board;
    return eval.compute(board, turn);
  }

  std::vector<Board> childBoards = generateMoves(board, turn);
  // childBoards = orderMoves(childBoards, turn);
  double bestValue = -1e100;
  Board bestBoard;
  for (auto child: childBoards) {
    double v = -negamax(child, depth - 1, -beta, -alpha, turn^1);
    if (v > bestValue) {
      bestValue = v;
      bestBoard = child;
    }
    alpha = std::max(v, alpha);
    if (alpha >= beta)
      break;
  }
  pv[depth] = bestBoard;
  return bestValue;
}

