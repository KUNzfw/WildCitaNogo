#include "board.h"

#include <algorithm>
#include <utility>

#include "bot.h"
#include "game.h"
#include "thirdparty/cpp-subprocess/subprocess.hpp"
using namespace subprocess;

Board::Board(int x, int y)
    : PaintView(x, y, CHESSBOARD_WIDTH, CHESSBOARD_WIDTH) {
  padding_ = (CHESSBOARD_WIDTH - (CHESSBOARD_SIZE - 1) * GRID_WIDTH) / 2;
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
    if (i == history_.size() - 1) {
      FillAACircle(x, y, CHESS_RADIUS + 2, SDL_Color{0xFF, 0, 0, 0xFF});
    }
    FillAACircle(x, y, CHESS_RADIUS, i % 2 == 0 ? CHESS_BLACK : CHESS_WHITE);
  }
}

void Board::OnShow(Context &context) {
  PaintView::OnShow(context);
  draw_board();
}

void Board::OnRender(Context &context) {
  if (mode_ != GAME_MODE_MULTI && (history_.size() % 2 == bot_first_ ? 0 : 1)) {
    if (!bot_running_) {
      bot_running_ = true;

      std::stringstream input{};
      input << history_.size() / 2 + 1 << ' ';
      if (bot_first_) input << -1 << ' ' << -1 << ' ';
      for (const auto &p : history_) {
        input << p.x << ' ' << p.y << ' ';
      }
      bot::SetInput(input.str());
      bot::PrepareRun();
      if (mode_ == GAME_MODE_SINGLE_HARD) {
        bot_thread_ = SDL_CreateThread(bot::RunHardBot, "HardBot", nullptr);
      } else if (mode_ == GAME_MODE_SINGLE_EASY) {
        bot_thread_ = SDL_CreateThread(bot::RunHardBot, "EasyBot", nullptr);
      }
    } else {
      if (bot::IsEnd() && bot_thread_ != nullptr) {
        SDL_WaitThread(bot_thread_, nullptr);
        bot_thread_ = nullptr;
        std::string res = bot::GetOutput();
        std::stringstream ss(res);
        Pos pos{};
        ss >> pos.x >> pos.y;
        place(pos);

        bot_running_ = false;

        check_game_end(context);
      }
    }
  }
  PaintView::OnRender(context);
}

void Board::OnMouseButtonDown(Context &context, Sint32 x, Sint32 y,
                              Uint8 button) {
  if (pause_ || bot_running_) return;
  if (button != SDL_BUTTON_LEFT) return;
  int tx = x - GetX(), ty = y - GetY();
  if (tx >= 0 && ty >= 0 && tx < CHESSBOARD_WIDTH && ty < CHESSBOARD_WIDTH) {
    int bx = (tx - padding_ + TOLERANCE) / GRID_WIDTH;
    int dx = (tx - padding_ + TOLERANCE) % GRID_WIDTH;
    int by = (ty - padding_ + TOLERANCE) / GRID_WIDTH;
    int dy = (ty - padding_ + TOLERANCE) % GRID_WIDTH;

    if (dx >= 0 && dx < TOLERANCE * 2 && dy >= 0 && dy < TOLERANCE * 2) {
      Pos p{bx, by};

      // avoid place stone to a same place
      if (std::find(history_.begin(), history_.end(), p) != history_.end()) {
        return;
      }

      // to make the game easier, player can place only if the place is valid
      if (!game::JudgeValid(history_, p)) {
        return;
      }

      place(bx, by);

      check_game_end(context);
    }
  }
}

void Board::OnMouseMotion(Context &context, Sint32 x, Sint32 y) {
  if (pause_ || bot_running_) return;
  int tx = x - GetX(), ty = y - GetY();
  if (tx >= 0 && ty >= 0 && tx < CHESSBOARD_WIDTH && ty < CHESSBOARD_WIDTH) {
    int bx = (tx - padding_ + TOLERANCE) / GRID_WIDTH;
    int dx = (tx - padding_ + TOLERANCE) % GRID_WIDTH;
    int by = (ty - padding_ + TOLERANCE) / GRID_WIDTH;
    int dy = (ty - padding_ + TOLERANCE) % GRID_WIDTH;
    Pos p{bx, by};
    if (!(dx >= 0 && dx < TOLERANCE * 2 && dy >= 0 && dy < TOLERANCE * 2)) {
      p = Pos{-1, -1};
    }

    // update only when the hint should be changed
    if (p == prev_hint_pos) {
      return;
    } else {
      prev_hint_pos = p;
    }

    if (p.x != -1) {
      // draw hint only when the place is empty
      if (std::find(history_.begin(), history_.end(), p) != history_.end()) {
        return;
      }

      // draw hint only when the place is valid
      if (!game::JudgeValid(history_, p)) {
        return;
      }

      draw_board();
      SDL_Color c = history_.size() % 2 != 0 ? CHESS_WHITE : CHESS_BLACK;
      c.a = 0xC0;
      DrawRect(padding_ + bx * GRID_WIDTH - TOLERANCE + 1,
               padding_ + by * GRID_WIDTH - TOLERANCE + 1, TOLERANCE * 2,
               TOLERANCE * 2, true, c);
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
    return -1;
  }
}
int Board::place(Pos pos) { return place(pos.x, pos.y); }
void Board::NewGame(GAME_MODE mode, bool bot_first) {
  pause_ = false;
  mode_ = mode;
  bot_first_ = bot_first;
  history_.clear();
}
void Board::PauseGame() { pause_ = true; }
void Board::UnpauseGame() { pause_ = false; }
bool Board::IsBotRunning() const { return bot_running_; }
void Board::SetOnGameEndListener(
    std::function<void(Context &context)> listener) {
  on_game_end_listener = std::move(listener);
}
void Board::check_game_end(Context &context) {
  int game_state = game::Judge(history_);
  if (game_state != 0) {
    if (game_state == 1) {
      game_result_ = GAME_WHITE_WIN;
    } else if (game_state == -1) {
      game_result_ = GAME_BLACK_WIN;
    }

    if (on_game_end_listener != nullptr) {
      on_game_end_listener(context);
    }
  }
}
GAME_RESULT Board::GetGameResult() const { return game_result_; }
void Board::ReadFromSlot(save::SLOT slot) {
  auto data{save::GetSlotData(slot)};
  NewGame(data.mode, data.bot_first);
  history_ = data.history;
}
void Board::SaveToSlot(save::SLOT slot) {
  save::GameData data{mode_, bot_first_, std::chrono::system_clock::now(),
                      history_};
  save::SaveToSlot(slot, data);
}
std::string GetModeName(GAME_MODE mode) {
  switch (mode) {
    case GAME_MODE_MULTI:
      return "multi-player";
    case GAME_MODE_SINGLE_EASY:
      return "single-player easy";
    case GAME_MODE_SINGLE_HARD:
      return "single-player hard";
    default:
      return "";
  }
}
