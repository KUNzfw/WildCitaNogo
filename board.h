#ifndef NOGO_BOARD_h_
#define NOGO_BOARD_h_

#include <functional>
#include <sstream>
#include <vector>

#include "ccgame.h"

using namespace ccgame;

constexpr int CHESSBOARD_SIZE = 9;
constexpr int CHESSBOARD_WIDTH = 400;
constexpr int GRID_WIDTH = 42;
constexpr int CHESS_RADIUS = 18;
constexpr int TOLERANCE = 12;
const SDL_Color GRID_COLOR{0, 0, 0, 0xFF};
const SDL_Color BACKGROUND_COLOR{0xF1, 0xCA, 0x17, 0xFF};
const SDL_Color CHESS_BLACK{0, 0, 0, 0xFF};
const SDL_Color CHESS_WHITE{0xFF, 0xFF, 0xFF, 0xFF};

struct Pos {
  int x, y;

  bool operator==(const Pos &rval) const { return x == rval.x && y == rval.y; }
};

enum GAME_MODE {
  GAME_MODE_MULTI,
  GAME_MODE_SINGLE_EASY,
  GAME_MODE_SINGLE_HARD
};

enum GAME_RESULT {
  GAME_BLACK_WIN,
  GAME_WHITE_WIN,
};

class Board : public PaintView {
 public:
  Board(int x, int y);

  void OnShow(Context &context) override;

  void OnRender(Context &context) override;

  void OnMouseButtonDown(Context &context, Sint32 x, Sint32 y,
                         Uint8 button) override;

  void OnMouseMotion(Context &context, Sint32 x, Sint32 y) override;

  void NewGame(GAME_MODE mode, bool bot_first);

  void PauseGame();
  void UnpauseGame();

  bool IsBotRunning() const;

  void SetOnGameEndListener(std::function<void(Context &context)> listener);

  GAME_RESULT GetGameResult() const;

 private:
  void draw_board();
  int place(int x, int y);
  int place(Pos pos);
  void check_game_end(Context &context);

  std::stringstream data_input{};
  std::stringstream data_output{};

  bool pause_{false};
  int padding_;
  GAME_MODE mode_{};
  bool bot_first_{};
  bool bot_running_{false};

  SDL_Thread *bot_thread_{nullptr};

  std::vector<Pos> history_;

  std::function<void(Context &)> on_game_end_listener{};

  GAME_RESULT game_result_{};
};

#endif