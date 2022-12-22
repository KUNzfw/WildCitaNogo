#ifndef NOGO_BOARD_h_
#define NOGO_BOARD_h_
#include "ccgame.h"
#include <vector>
using namespace ccgame;

constexpr int CHESSBOARD_SIZE = 9;
constexpr int CHESSBOARD_WIDTH = 400;
constexpr int GRID_WIDTH = 42;
constexpr int CHESS_RADIUS = 18;
constexpr int TOLERRANCE = 12;
const SDL_Color GRID_COLOR{0, 0, 0, 0xFF};
const SDL_Color BACKGROUND_COLOR{0xF1, 0xCA, 0x17, 0xFF};
const SDL_Color CHESS_BLACK{0, 0, 0, 0xFF};
const SDL_Color CHESS_WHITE{0xFF, 0xFF, 0xFF, 0xFF};

struct Pos {
  int x, y;
  bool operator==(const Pos &rval) { return x == rval.x && y == rval.y; }
};

class Board : public PaintView {
public:
  Board(int x, int y);
  void OnMouseButtonDown(Context &context, Sint32 x, Sint32 y,
                         Uint8 button) override;

  void OnMouseMotion(Context &context, Sint32 x, Sint32 y) override;

private:
  void draw_board();
  int place(int x, int y);
  int padding_;
  std::vector<Pos> history_;
};
#endif