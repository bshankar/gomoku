#include "evaluation.hpp"

double Evaluation::compute(Board board, int turn) {
  if (board.winner() == turn)
    return 1e99;
  if (board.winner() == (turn ^ 1))
    return -1e99;

  double eval = 0;
  eval += center(board, turn);
  eval += partials(board, turn);
  return eval;
}

double Evaluation::center(Board& board, int turn) {
  // center control
  double eval = 0;

  for (int h = 0; h < 19; ++h)
    eval += rowCenter(board.houses[turn][h], h) -
      rowCenter(board.houses[turn ^ 1][h], h);
  
  return eval;
}

double Evaluation::rowCenter(board_t house, int row) {
  double eval = 0;
  int col = 0;
  while (house) {
    if (house & 1) {
      eval += 38 - std::abs(row - 19/2.0) - std::abs(col - 19/2.0);
      house--;
    }
    int moveBy = __builtin_ffs(house) - 1;
    col += moveBy;
    house >>= moveBy;
  }
  return eval;
}

double Evaluation::partials(Board &board, int turn) {
  double eval = 0;
  for (int h = 0; h < 112; ++h)
    eval += compareHouses(board.houses[turn][h],
                          board.houses[turn ^ 1][h]);
  return eval;
}

double Evaluation::compareHouses(board_t h1, board_t h2) {
  board_t bitmask = 0b11111;
  double eval = 0;
  
  while (h1 || h2) {
    auto res = h1 & bitmask,
      resOpp = h2 & bitmask;

    if (res != 0 && resOpp == 0)
      eval += 100*(1 << __builtin_popcount(res));
    else if (res == 0 && resOpp != 0)
      eval -= 100*(1 << __builtin_popcount(resOpp));

    h1 >>= 1;
    h2 >>= 1;
  }
  return eval;
}
