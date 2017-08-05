#include "evaluation.hpp"
#include <cmath>

double Evaluation::compute(Board board, int turn) {
  if (board.winner() == turn)
    return 1e99;
  if (board.winner() == (turn ^ 1))
    return -1e99;

  double eval = 0;
  eval = center(board, eval, turn);
  eval = partials(board, eval, turn);
  return eval;
}

double Evaluation::center(Board& board, double eval, int turn) {
  // center control
  for (int r = 0; r < 19; ++r)
    for (int c = 0; c < 19; ++c) {
      if (board.rows[turn][r][c])
        eval += 38 - std::abs(r - 19/2.0) - std::abs(c - 19/2.0);
      else if (board.rows[turn ^ 1][r][c])
        eval -= 38 - std::abs(r - 19/2.0) - std::abs(c - 19/2.0);
    }
  return eval;
}

double Evaluation::partials(Board &board, double eval, int turn) {
  
  for (int r = 0; r < 19; ++r) {
    std::bitset<19> bitmask = 0b11111;
    for (int i = 0; i < 19 - 5; ++i) {
      // rows
      auto resTurn = board.rows[turn][r] & bitmask;
      auto resOpp = board.rows[turn ^ 1][r] & bitmask;
      if (resTurn != 0 && resOpp == 0)
        eval += 100*pow(2, resTurn.count());
      else if (resTurn == 0 && resOpp != 0)
        eval -= 100*pow(2, resOpp.count());

      // colns
      resTurn = board.cols[turn][r] & bitmask;
      resOpp = board.cols[turn ^ 1][r] & bitmask;
      if (resTurn != 0 && resOpp == 0)
        eval += 100*pow(2, resTurn.count());
      else if (resTurn == 0 && resOpp != 0)
        eval -= 100*pow(2, resOpp.count());
      
      bitmask <<= 1;
    }
  }

  for (int d = 0; d < 37; ++d) {
    std::bitset<19> bitmask = 0b11111;
    for (int i = 0; i < 19 - 5; ++i) {
      // diagonals
      auto resTurn = board.diag[turn][d] & bitmask;
      auto resOpp = board.diag[turn ^ 1][d] & bitmask;
      if (resTurn != 0 && resOpp == 0)
        eval += 100*pow(2, resTurn.count());
      else if (resTurn == 0 && resOpp != 0)
        eval -= 100*pow(2, resOpp.count());
      
      // anti diagonals
      resTurn = board.antiDiag[turn][d] & bitmask;
      resOpp = board.antiDiag[turn][d] & bitmask;
      if (resTurn != 0 && resOpp == 0)
        eval += 100*pow(2, resTurn.count());
      else if (resTurn == 0 && resOpp != 0)
        eval -= 100*pow(2, resOpp.count());
      
      bitmask <<= 1;
    }
  }
  return eval;
}
