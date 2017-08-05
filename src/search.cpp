#include "search.hpp"
using std::vector;

Search::Search() {
  
}

vector<Board> Search::generateMoves(const Board& board, int turn) {
  vector<Board> moves;
  for (int r = 0; r < 19; ++r)
    for (int c = 0; c < 19; ++c)
      if (!(board.houses[0][r] & (1 << c)) && !(board.houses[1][r] & (1 << c))) {
        Board tmp = board;
        tmp.place(r, c, turn);
        moves.push_back(tmp);
      } 
  return moves;
}

double Search::negamax(Board& board, int depth,
                       double alpha, double beta, int turn) {

  if (!depth || board.winner() != -1) {
    Evaluation eval;
    pv[depth] = board;
    return eval.compute(board, turn);
  }

  std::vector<Board> childBoards = generateMoves(board, turn);
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

