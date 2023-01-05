#include <filesystem>
#include <iomanip>
#include <iostream>
#include <random>

#include "board.h"
#include "ccgame.h"
#include "save.h"

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

SDL_Color COLOR_QUNQIN{0x17, 0x72, 0xB4, 0xFF};
SDL_Color COLOR_TANGCILAN{0x11, 0x65, 0x9A, 0xFF};
SDL_Color COLOR_BOLUOHONG{0xFC, 0x79, 0x30, 0xFF};
SDL_Color COLOR_YANHEHONG{0xFC, 0x63, 0x15, 0xFF};

// to manage all the page
enum PAGE {
  PAGE_MENU,
  PAGE_NEW_GAME,
  PAGE_LOAD_GAME,
  PAGE_GAME,
  PAGE_PAUSE,
  PAGE_SAVE,
  PAGE_END
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    SDL context{CCGAME_INIT_VIDEO, CCGAME_INIT_TTF};

    auto base_path_raw{SDL_GetBasePath()};
    const auto base_path{std::filesystem::path(base_path_raw)};
    SDL_free(base_path_raw);

    Window window("Nogo", WINDOW_WIDTH, WINDOW_HEIGHT);

    Font joystix36((base_path / "assets" / "joystix.ttf").string(), 36);
    Font joystix28((base_path / "assets" / "joystix.ttf").string(), 28);
    Font joystix20((base_path / "assets" / "joystix.ttf").string(), 20);
    Font joystix14((base_path / "assets" / "joystix.ttf").string(), 14);

    window.SetBackgroundColor(0x86, 0x9D, 0x9D, 0xFF);

    Board board(120, 40);

    // PAGE_MENU start
    RectView button_menu_new_game(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_QUNQIN);
    button_menu_new_game.SetAlign(CCGAME_ALIGN_CENTER);

    button_menu_new_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_menu_new_game.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            // show next page
            window.HideAllGroup();
            window.ShowGroup(PAGE_NEW_GAME);
          }
        });
    TextView text_menu_new_game("NEW GAME", &joystix28,
                                button_menu_new_game.GetX(),
                                button_menu_new_game.GetY());
    text_menu_new_game.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_menu_load_game(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 2, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_QUNQIN);
    button_menu_load_game.SetAlign(CCGAME_ALIGN_CENTER);

    button_menu_load_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_menu_load_game.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_LOAD_GAME);
          }
        });
    TextView text_menu_load_game("LOAD GAME", &joystix28,
                                 button_menu_load_game.GetX(),
                                 button_menu_load_game.GetY());
    text_menu_load_game.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_menu_quit_game(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, SDL_Color{0xCC, 0x22, 0x22, 0xFF});
    button_menu_quit_game.SetAlign(CCGAME_ALIGN_CENTER);
    button_menu_quit_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_menu_quit_game.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.Quit();
          }
        });

    TextView text_menu_quit_game("QUIT", &joystix28,
                                 button_menu_quit_game.GetX(),
                                 button_menu_quit_game.GetY());
    text_menu_quit_game.SetAlign(CCGAME_ALIGN_CENTER);

    window.RegisterView(&button_menu_new_game, PAGE_MENU);
    window.RegisterView(&text_menu_new_game, PAGE_MENU);
    window.RegisterView(&button_menu_load_game, PAGE_MENU);
    window.RegisterView(&text_menu_load_game, PAGE_MENU);
    window.RegisterView(&button_menu_quit_game, PAGE_MENU);
    window.RegisterView(&text_menu_quit_game, PAGE_MENU);
    // PAGE_MENU end

    // PAGE_NEW_GAME start
    RectView button_new_game_multi(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_QUNQIN);
    button_new_game_multi.SetAlign(CCGAME_ALIGN_CENTER);

    button_new_game_multi.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_new_game_multi.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            // show next page
            board.NewGame(GAME_MODE_MULTI, false);
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
          }
        });
    TextView text_new_game_multi("MULTI", &joystix28,
                                 button_new_game_multi.GetX(),
                                 button_new_game_multi.GetY());
    text_new_game_multi.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_new_game_single_easy(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 2, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_TANGCILAN);
    button_new_game_single_easy.SetAlign(CCGAME_ALIGN_CENTER);

    button_new_game_single_easy.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_new_game_single_easy.IsInside(x, y)) {
            if (button == SDL_BUTTON_LEFT) {
              board.NewGame(GAME_MODE_SINGLE_EASY, false);
            } else if (button == SDL_BUTTON_RIGHT) {
              board.NewGame(GAME_MODE_SINGLE_EASY, true);
            }
            // show next page
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
          }
        });
    TextView text_new_game_single_easy("EASY", &joystix28,
                                       button_new_game_single_easy.GetX(),
                                       button_new_game_single_easy.GetY());
    text_new_game_single_easy.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_new_game_single_hard(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_BOLUOHONG);
    button_new_game_single_hard.SetAlign(CCGAME_ALIGN_CENTER);

    button_new_game_single_hard.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_new_game_single_hard.IsInside(x, y)) {
            if (button == SDL_BUTTON_LEFT) {
              board.NewGame(GAME_MODE_SINGLE_HARD, false);
            } else if (button == SDL_BUTTON_RIGHT) {
              board.NewGame(GAME_MODE_SINGLE_HARD, true);
            }
            // show next page
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
          }
        });
    TextView text_new_game_single_hard("HARD", &joystix28,
                                       button_new_game_single_hard.GetX(),
                                       button_new_game_single_hard.GetY());
    text_new_game_single_hard.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_new_game_hint1("left click to player first", &joystix14,
                                 WINDOW_WIDTH / 2, WINDOW_HEIGHT - 30);
    text_new_game_hint1.SetAlign(CCGAME_ALIGN_CENTER);
    TextView text_new_game_hint2("right click to bot first", &joystix14,
                                 WINDOW_WIDTH / 2, WINDOW_HEIGHT - 15);
    text_new_game_hint2.SetAlign(CCGAME_ALIGN_CENTER);

    TextView button_new_game_back("<", &joystix28, 15, 15);
    button_new_game_back.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_new_game_back.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&button_new_game_multi, PAGE_NEW_GAME);
    window.RegisterView(&text_new_game_multi, PAGE_NEW_GAME);
    window.RegisterView(&button_new_game_single_easy, PAGE_NEW_GAME);
    window.RegisterView(&text_new_game_single_easy, PAGE_NEW_GAME);
    window.RegisterView(&button_new_game_single_hard, PAGE_NEW_GAME);
    window.RegisterView(&text_new_game_single_hard, PAGE_NEW_GAME);
    window.RegisterView(&text_new_game_hint1, PAGE_NEW_GAME);
    window.RegisterView(&text_new_game_hint2, PAGE_NEW_GAME);
    window.RegisterView(&button_new_game_back, PAGE_NEW_GAME);
    // PAGE_NEW_GAME end

    // PAGE_LOAD_GAME start
    TextView button_load_game_back("<", &joystix28, 15, 15);
    button_load_game_back.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_load_game_back.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    // slot1
    RectView button_load_game_slot1(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 3 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_YANHEHONG);
    button_load_game_slot1.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot1_empty("SLOT 1", &joystix28,
                                        button_load_game_slot1.GetX(),
                                        button_load_game_slot1.GetY());
    text_load_game_slot1_empty.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot1_time("SLOT 1", &joystix20,
                                       button_load_game_slot1.GetX(),
                                       button_load_game_slot1.GetY() - 10);
    text_load_game_slot1_time.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot1_mode("SLOT 1", &joystix20,
                                       button_load_game_slot1.GetX(),
                                       button_load_game_slot1.GetY() + 10);
    text_load_game_slot1_mode.SetAlign(CCGAME_ALIGN_CENTER);

    bool slot1_available{false};
    button_load_game_slot1.SetOnShowListener([&](Context &context) {
      if (save::CheckSlotExist(save::SLOT1)) {
        window.HideView(&text_load_game_slot1_empty);
        auto data{save::GetSlotInfo(save::SLOT1)};
        auto t_c{std::chrono::system_clock::to_time_t(data.time)};
        std::stringstream time_string;
        time_string << std::put_time(std::localtime(&t_c), "%F %T");
        text_load_game_slot1_time.SetText(context, time_string.str());
        text_load_game_slot1_mode.SetText(context, GetModeName(data.mode));
        slot1_available = true;
      } else {
        window.HideView(&text_load_game_slot1_time);
        window.HideView(&text_load_game_slot1_mode);
      }
    });

    button_load_game_slot1.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (slot1_available && button_load_game_slot1.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            board.ReadFromSlot(save::SLOT1);
            window.ShowGroup(PAGE_GAME);
          }
        });
    // slot1 end

    // slot2
    RectView button_load_game_slot2(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 2, WINDOW_WIDTH / 3 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_YANHEHONG);
    button_load_game_slot2.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot2_empty("SLOT 2", &joystix28,
                                        button_load_game_slot2.GetX(),
                                        button_load_game_slot2.GetY());
    text_load_game_slot2_empty.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot2_time("SLOT 2", &joystix20,
                                       button_load_game_slot2.GetX(),
                                       button_load_game_slot2.GetY() - 10);
    text_load_game_slot2_time.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot2_mode("SLOT 2", &joystix20,
                                       button_load_game_slot2.GetX(),
                                       button_load_game_slot2.GetY() + 10);
    text_load_game_slot2_mode.SetAlign(CCGAME_ALIGN_CENTER);

    bool slot2_available{false};
    button_load_game_slot2.SetOnShowListener([&](Context &context) {
      if (save::CheckSlotExist(save::SLOT2)) {
        window.HideView(&text_load_game_slot2_empty);
        auto data{save::GetSlotInfo(save::SLOT2)};
        auto t_c{std::chrono::system_clock::to_time_t(data.time)};
        std::stringstream time_string;
        time_string << std::put_time(std::localtime(&t_c), "%F %T");
        text_load_game_slot2_time.SetText(context, time_string.str());
        text_load_game_slot2_mode.SetText(context, GetModeName(data.mode));
        slot2_available = true;
      } else {
        window.HideView(&text_load_game_slot2_time);
        window.HideView(&text_load_game_slot2_mode);
      }
    });

    button_load_game_slot2.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (slot2_available && button_load_game_slot2.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            board.ReadFromSlot(save::SLOT2);
            window.ShowGroup(PAGE_GAME);
          }
        });
    // slot2 end

    // slot3
    RectView button_load_game_slot3(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3, WINDOW_WIDTH / 3 * 2,
                 WINDOW_HEIGHT / 6},
        true, COLOR_YANHEHONG);
    button_load_game_slot3.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot3_empty("SLOT 3", &joystix28,
                                        button_load_game_slot3.GetX(),
                                        button_load_game_slot3.GetY());
    text_load_game_slot3_empty.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot3_time("SLOT 3", &joystix20,
                                       button_load_game_slot3.GetX(),
                                       button_load_game_slot3.GetY() - 10);
    text_load_game_slot3_time.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_load_game_slot3_mode("SLOT 3", &joystix20,
                                       button_load_game_slot3.GetX(),
                                       button_load_game_slot3.GetY() + 10);
    text_load_game_slot3_mode.SetAlign(CCGAME_ALIGN_CENTER);

    bool slot3_available{false};
    button_load_game_slot3.SetOnShowListener([&](Context &context) {
      if (save::CheckSlotExist(save::SLOT3)) {
        window.HideView(&text_load_game_slot3_empty);
        auto data{save::GetSlotInfo(save::SLOT3)};
        auto t_c{std::chrono::system_clock::to_time_t(data.time)};
        std::stringstream time_string;
        time_string << std::put_time(std::localtime(&t_c), "%F %T");
        text_load_game_slot3_time.SetText(context, time_string.str());
        text_load_game_slot3_mode.SetText(context, GetModeName(data.mode));
        slot3_available = true;
      } else {
        window.HideView(&text_load_game_slot3_time);
        window.HideView(&text_load_game_slot3_mode);
      }
    });

    button_load_game_slot3.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (slot3_available && button_load_game_slot3.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            board.ReadFromSlot(save::SLOT3);
            window.ShowGroup(PAGE_GAME);
          }
        });
    // slot3 end

    window.RegisterView(&button_load_game_back, PAGE_LOAD_GAME);
    window.RegisterView(&button_load_game_slot1, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot1_empty, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot1_time, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot1_mode, PAGE_LOAD_GAME);
    window.RegisterView(&button_load_game_slot2, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot2_empty, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot2_time, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot2_mode, PAGE_LOAD_GAME);
    window.RegisterView(&button_load_game_slot3, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot3_empty, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot3_time, PAGE_LOAD_GAME);
    window.RegisterView(&text_load_game_slot3_mode, PAGE_LOAD_GAME);
    // PAGE_LOAD_GAME end

    // PAGE_GAME start
    TextView button_game_pause("PAUSE", &joystix14, 10, 10);

    button_game_pause.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_game_pause.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            window.ShowGroup(PAGE_PAUSE);
            board.PauseGame();
          }
        });

    TextView text_game_bot_thinking("Player's turn", &joystix14,
                                    WINDOW_WIDTH / 2, WINDOW_HEIGHT - 10);
    text_game_bot_thinking.SetAlign(CCGAME_ALIGN_CENTER);
    bool text_bot_thinking_state{false};
    text_game_bot_thinking.SetOnRenderListener([&](Context &context) {
      // update text only when the text should be updated
      if (board.IsBotRunning() && !text_bot_thinking_state) {
        text_game_bot_thinking.SetText(context, "Bot thinking...");
        text_bot_thinking_state = true;
      }
      if (!board.IsBotRunning() && text_bot_thinking_state) {
        text_game_bot_thinking.SetText(context, "Player's turn");
        text_bot_thinking_state = false;
      }
    });

    window.RegisterView(&button_game_pause, PAGE_GAME);
    window.RegisterView(&board, PAGE_GAME);
    window.RegisterView(&text_game_bot_thinking, PAGE_GAME);
    // PAGE_GAME end

    // PAGE_PAUSE start
    RectView rect_pause_blur(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                             SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect_pause_blur.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect_pause_blur.SetAlpha(0xCC);

    TextView button_pause_continue("CONTINUE", &joystix28, WINDOW_WIDTH / 2,
                                   WINDOW_HEIGHT / 4);
    button_pause_continue.SetAlign(CCGAME_ALIGN_CENTER);
    button_pause_continue.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_pause_continue.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            board.UnpauseGame();
            window.HideGroup(PAGE_PAUSE);
          }
        });

    TextView button_pause_save("SAVE", &joystix28, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT / 4 * 2);
    button_pause_save.SetAlign(CCGAME_ALIGN_CENTER);
    button_pause_save.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_pause_save.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            window.HideGroup(PAGE_PAUSE);
            window.ShowGroup(PAGE_SAVE);
          }
        });

    TextView button_pause_exit("EXIT", &joystix28, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT / 4 * 3);
    button_pause_exit.SetAlign(CCGAME_ALIGN_CENTER);
    button_pause_exit.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_pause_exit.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&rect_pause_blur, PAGE_PAUSE);
    window.RegisterView(&button_pause_continue, PAGE_PAUSE);
    window.RegisterView(&button_pause_save, PAGE_PAUSE);
    window.RegisterView(&button_pause_exit, PAGE_PAUSE);
    // PAGE_PAUSE end

    // PAGE_SAVE start
    RectView rect_save_blur(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                            SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect_save_blur.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect_save_blur.SetAlpha(0xCC);

    TextView button_save_back("<", &joystix28, 15, 15);
    button_save_back.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_save_back.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            window.HideGroup(PAGE_SAVE);
            window.ShowGroup(PAGE_PAUSE);
          }
        });

    TextView text_save_hint("Save to", &joystix36, WINDOW_WIDTH / 2,
                            WINDOW_HEIGHT / 6);
    text_save_hint.SetAlign(CCGAME_ALIGN_CENTER);

    TextView button_save_slot1("SLOT1", &joystix28, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT / 5 * 2);
    button_save_slot1.SetAlign(CCGAME_ALIGN_CENTER);
    button_save_slot1.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_save_slot1.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            board.SaveToSlot(save::SLOT1);
            window.HideGroup(PAGE_SAVE);
            window.ShowGroup(PAGE_PAUSE);
          }
        });

    TextView button_save_slot2("SLOT2", &joystix28, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT / 5 * 3);
    button_save_slot2.SetAlign(CCGAME_ALIGN_CENTER);
    button_save_slot2.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_save_slot2.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            board.SaveToSlot(save::SLOT2);
            window.HideGroup(PAGE_SAVE);
            window.ShowGroup(PAGE_PAUSE);
          }
        });

    TextView button_save_slot3("SLOT3", &joystix28, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT / 5 * 4);
    button_save_slot3.SetAlign(CCGAME_ALIGN_CENTER);
    button_save_slot3.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_save_slot3.IsInside(x, y) && button == SDL_BUTTON_LEFT) {
            board.SaveToSlot(save::SLOT3);
            window.HideGroup(PAGE_SAVE);
            window.ShowGroup(PAGE_PAUSE);
          }
        });

    window.RegisterView(&rect_save_blur, PAGE_SAVE);
    window.RegisterView(&button_save_back, PAGE_SAVE);
    window.RegisterView(&text_save_hint, PAGE_SAVE);
    window.RegisterView(&button_save_slot1, PAGE_SAVE);
    window.RegisterView(&button_save_slot2, PAGE_SAVE);
    window.RegisterView(&button_save_slot3, PAGE_SAVE);
    // PAGE_SAVE end

    // PAGE_END start
    RectView rect_end_blur(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                           SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect_end_blur.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect_end_blur.SetAlpha(0xCC);

    TextView text_end_result("TEMP", &joystix28, WINDOW_WIDTH / 2,
                             WINDOW_HEIGHT / 2);
    text_end_result.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_end_hint("Click to return to main menu.", &joystix20,
                           WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2);
    text_end_hint.SetAlign(CCGAME_ALIGN_CENTER);
    text_end_hint.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          window.HideAllGroup();
          window.ShowGroup(PAGE_MENU);
        });

    board.SetOnGameEndListener([&](Context &context) {
      window.ShowGroup(PAGE_END);
      GAME_RESULT result{board.GetGameResult()};
      if (result == GAME_WHITE_WIN) {
        text_end_result.SetText(context, "WHITE WIN!");
      } else if (result == GAME_BLACK_WIN) {
        text_end_result.SetText(context, "BLACK WIN!");
      }
      board.PauseGame();
    });

    window.RegisterView(&rect_end_blur, PAGE_END);
    window.RegisterView(&text_end_result, PAGE_END);
    window.RegisterView(&text_end_hint, PAGE_END);
    // PAGE_END end

    // init status
    window.HideAllGroup();
    window.ShowGroup(PAGE_MENU);

    context.RegisterWindow(window);

    window.Start();

    return 0;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
}