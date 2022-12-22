#include "board.h"
#include <algorithm>
Board::Board(int x, int y)
    : PaintView(x, y, CHESSBOARD_WIDTH, CHESSBOARD_WIDTH) {
  padding_ = (CHESSBOARD_WIDTH - (CHESSBOARD_SIZE - 1) * GRID_WIDTH) / 2;
  draw_board();
}
void Board::draw_board() {
  int length = CHESSBOARD_WIDTH - padding_ * 2 + 1;

  SetBackgroundColor(BACKGROUND_COLOR);

  for (int i = 0; i < CHESSBOARD_SIZE; i++) {
    DrawRect(padding_, padding_ + i * GRID_WIDTH, length, 2, true, GRID_COLOR);
  }
  for (int i = 0; i < CHESSBOARD_SIZE; i++) {
    DrawRect(padding_ + i * GRID_WIDTH, padding_, 2, length, true, GRID_COLOR);
  }

  for (int i = 0; i < history_.size(); i++) {
    int x = history_[i].x * GRID_WIDTH + padding_;
    int y = history_[i].y * GRID_WIDTH + padding_;
    DrawAACircle(x, y, CHESS_RADIUS, i % 2 == 0 ? CHESS_BLACK : CHESS_WHITE);
  }
}
void Board::OnMouseButtonDown(Context &context, Sint32 x, Sint32 y,
                              Uint8 button) {
  if (button != SDL_BUTTON_LEFT)
    return;
  int tx = x - GetX(), ty = y - GetY();
  if (tx >= 0 && ty >= 0 && tx < CHESSBOARD_WIDTH && ty < CHESSBOARD_WIDTH) {
    int bx = (tx - padding_ + TOLERRANCE) / GRID_WIDTH;
    int dx = (tx - padding_ + TOLERRANCE) % GRID_WIDTH;
    int by = (ty - padding_ + TOLERRANCE) / GRID_WIDTH;
    int dy = (ty - padding_ + TOLERRANCE) % GRID_WIDTH;
    if (dx >= 0 && dx < TOLERRANCE * 2 && dy >= 0 && dy < TOLERRANCE * 2) {
      place(bx, by);
    }
  }
}
void Board::OnMouseMotion(Context &context, Sint32 x, Sint32 y) {
  int tx = x - GetX(), ty = y - GetY();
  if (tx >= 0 && ty >= 0 && tx < CHESSBOARD_WIDTH && ty < CHESSBOARD_WIDTH) {
    int bx = (tx - padding_ + TOLERRANCE) / GRID_WIDTH;
    int dx = (tx - padding_ + TOLERRANCE) % GRID_WIDTH;
    int by = (ty - padding_ + TOLERRANCE) / GRID_WIDTH;
    int dy = (ty - padding_ + TOLERRANCE) % GRID_WIDTH;
    if (dx >= 0 && dx < TOLERRANCE * 2 && dy >= 0 && dy < TOLERRANCE * 2) {
      Pos p{bx, by};
      if (std::find(history_.begin(), history_.end(), p) != history_.end())
        return;
      draw_board();
      SDL_Color c = history_.size() % 2 != 0 ? CHESS_WHITE : CHESS_BLACK;
      c.a = 0xC0;
      DrawRect(padding_ + bx * GRID_WIDTH - TOLERRANCE + 1,
               padding_ + by * GRID_WIDTH - TOLERRANCE + 1, TOLERRANCE * 2,
               TOLERRANCE * 2, true, c);
    } else {
      draw_board();
    }
  }
}
int Board::place(int x, int y) {
  Pos p{x, y};
  if (std::find(history_.begin(), history_.end(), p) == history_.end()) {
    history_.emplace_back(p);
    draw_board();
    return 0;
  } else {
    return 1;
  }
}