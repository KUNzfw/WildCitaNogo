#include <SDL_mouse.h>
#include <SDL_pixels.h>

#include <iostream>
#include <random>

#include "board.h"
#include "ccgame.h"
#include "ccgame/src/view.h"
#include "ccgame/src/view/textview.h"

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

// to manage all the page
enum PAGE {
  PAGE_MENU,
  PAGE_NEW_GAME,
  PAGE_LOAD_GAME,
  PAGE_GAME,
  PAGE_PAUSE,
  PAGE_END
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    SDL context{CCGAME_INIT_VIDEO, CCGAME_INIT_TTF};

    Window window("Snake", WINDOW_WIDTH, WINDOW_HEIGHT);

    Font joystix28(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 28);
    Font joystix14(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 14);

    window.SetBackgroundColor(0xAA, 0xAA, 0xAA, 0xFF);

    Board board(120, 40);

    // PAGE_MENU start
    RectView button_new_game(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4,
                                      WINDOW_WIDTH / 5 * 2, WINDOW_HEIGHT / 6},
                             true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button_new_game.SetAlign(CCGAME_ALIGN_CENTER);

    button_new_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_new_game.IsInside(x, y)) {
            // show next page
            window.HideAllGroup();
            window.ShowGroup(PAGE_NEW_GAME);
          }
        });
    TextView text_new_game("NEW GAME", &joystix28, button_new_game.GetX(),
                           button_new_game.GetY());
    text_new_game.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_load_game(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 2,
                                       WINDOW_WIDTH / 5 * 2, WINDOW_HEIGHT / 6},
                              true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button_load_game.SetAlign(CCGAME_ALIGN_CENTER);

    button_load_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_load_game.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_LOAD_GAME);
          }
        });
    TextView text_load_game("LOAD GAME", &joystix28, button_load_game.GetX(),
                            button_load_game.GetY());
    text_load_game.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_quit_game(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3,
                                       WINDOW_WIDTH / 5 * 2, WINDOW_HEIGHT / 6},
                              true, SDL_Color{0xCC, 0x22, 0x22, 0xFF});
    button_quit_game.SetAlign(CCGAME_ALIGN_CENTER);
    button_quit_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_quit_game.IsInside(x, y)) {
            window.Quit();
          }
        });

    TextView text_quit_game("QUIT", &joystix28, button_quit_game.GetX(),
                            button_quit_game.GetY());
    text_quit_game.SetAlign(CCGAME_ALIGN_CENTER);

    window.RegisterView(&button_new_game, PAGE_MENU);
    window.RegisterView(&text_new_game, PAGE_MENU);
    window.RegisterView(&button_load_game, PAGE_MENU);
    window.RegisterView(&text_load_game, PAGE_MENU);
    window.RegisterView(&button_quit_game, PAGE_MENU);
    window.RegisterView(&text_quit_game, PAGE_MENU);
    // PAGE_MENU end

    // PAGE_NEW_GAME start
    RectView button_multi(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4,
                                   WINDOW_WIDTH / 5 * 2, WINDOW_HEIGHT / 6},
                          true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button_multi.SetAlign(CCGAME_ALIGN_CENTER);

    button_multi.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_multi.IsInside(x, y)) {
            // show next page
            board.NewGame(GAME_MODE_MULTI, false);
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
          }
        });
    TextView text_multi("MULTI", &joystix28, button_multi.GetX(),
                        button_multi.GetY());
    text_multi.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_single_easy(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 2, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button_single_easy.SetAlign(CCGAME_ALIGN_CENTER);

    button_single_easy.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_single_easy.IsInside(x, y)) {
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
    TextView text_single_easy("EASY", &joystix28, button_single_easy.GetX(),
                              button_single_easy.GetY());
    text_single_easy.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button_single_hard(
        SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3, WINDOW_WIDTH / 5 * 2,
                 WINDOW_HEIGHT / 6},
        true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button_single_hard.SetAlign(CCGAME_ALIGN_CENTER);

    button_single_hard.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_single_hard.IsInside(x, y)) {
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
    TextView text_single_hard("HARD", &joystix28, button_single_hard.GetX(),
                              button_single_hard.GetY());
    text_single_hard.SetAlign(CCGAME_ALIGN_CENTER);

    TextView text_hint1("left click to player first", &joystix14,
                        WINDOW_WIDTH / 2, WINDOW_HEIGHT - 30);
    text_hint1.SetAlign(CCGAME_ALIGN_CENTER);
    TextView text_hint2("right click to bot first", &joystix14,
                        WINDOW_WIDTH / 2, WINDOW_HEIGHT - 15);
    text_hint2.SetAlign(CCGAME_ALIGN_CENTER);

    TextView button_back_new_game("<", &joystix28, 15, 15);
    button_back_new_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_back_new_game.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&button_multi, PAGE_NEW_GAME);
    window.RegisterView(&text_multi, PAGE_NEW_GAME);
    window.RegisterView(&button_single_easy, PAGE_NEW_GAME);
    window.RegisterView(&text_single_easy, PAGE_NEW_GAME);
    window.RegisterView(&button_single_hard, PAGE_NEW_GAME);
    window.RegisterView(&text_single_hard, PAGE_NEW_GAME);
    window.RegisterView(&text_hint1, PAGE_NEW_GAME);
    window.RegisterView(&text_hint2, PAGE_NEW_GAME);
    window.RegisterView(&button_back_new_game, PAGE_NEW_GAME);
    // PAGE_NEW_GAME end

    // PAGE_LOAD_GAME start
    TextView button_back_load_game("<", &joystix28, 15, 15);
    button_back_load_game.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_back_load_game.IsInside(x, y) &&
              button == SDL_BUTTON_LEFT) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&button_back_load_game, PAGE_LOAD_GAME);
    // PAGE_LOAD_GAME end

    // PAGE_GAME start
    TextView button_pause("PAUSE", &joystix14, 10, 10);

    button_pause.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_pause.IsInside(x, y)) {
            window.ShowGroup(PAGE_PAUSE);
            board.PauseGame();
          }
        });

    TextView text_bot_thinking("Bot thinking...", &joystix14, WINDOW_WIDTH / 2,
                               WINDOW_HEIGHT - 10);
    text_bot_thinking.SetAlign(CCGAME_ALIGN_CENTER);
    bool text_bot_thinking_state{false};
    text_bot_thinking.SetOnRenderListener([&](Context &context) {
      if (board.IsBotRunning() && !text_bot_thinking_state) {
        text_bot_thinking.SetText(context, "Bot thinking...");
        text_bot_thinking_state = true;
      }
      if (!board.IsBotRunning() && text_bot_thinking_state) {
        text_bot_thinking.SetText(context, "Player's turn");
        text_bot_thinking_state = false;
      }
    });

    window.RegisterView(&button_pause, PAGE_GAME);
    window.RegisterView(&board, PAGE_GAME);
    window.RegisterView(&text_bot_thinking, PAGE_GAME);
    // PAGE_GAME end

    // PAGE_PAUSE start
    RectView rect_blur_pause(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                             SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect_blur_pause.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect_blur_pause.SetAlpha(0xCC);

    TextView button_continue("CONTINUE", &joystix28, WINDOW_WIDTH / 2,
                             WINDOW_HEIGHT / 4);
    button_continue.SetAlign(CCGAME_ALIGN_CENTER);
    button_continue.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_continue.IsInside(x, y)) {
            board.UnpauseGame();
            window.HideGroup(PAGE_PAUSE);
          }
        });

    TextView button_save("SAVE", &joystix28, WINDOW_WIDTH / 2,
                         WINDOW_HEIGHT / 4 * 2);
    button_save.SetAlign(CCGAME_ALIGN_CENTER);
    button_save.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_save.IsInside(x, y)) {
          }
        });

    TextView button_exit("EXIT", &joystix28, WINDOW_WIDTH / 2,
                         WINDOW_HEIGHT / 4 * 3);
    button_exit.SetAlign(CCGAME_ALIGN_CENTER);
    button_exit.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button_exit.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&rect_blur_pause, PAGE_PAUSE);
    window.RegisterView(&button_continue, PAGE_PAUSE);
    window.RegisterView(&button_save, PAGE_PAUSE);
    window.RegisterView(&button_exit, PAGE_PAUSE);
    // PAGE_PAUSE end

    // PAGE_END start
    RectView rect_blur_end(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                           SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect_blur_end.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect_blur_end.SetAlpha(0xCC);

    Font joystix20(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 20);
    TextView text_end("YOU DIED!PRESS ANY KEY TO RESTART!", &joystix20,
                      WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    text_end.SetAlign(CCGAME_ALIGN_CENTER);
    text_end.SetOnKeydownListener([&](Context &context, SDL_Keycode keycode) {
      window.HideGroup(PAGE_END);
    });

    window.RegisterView(&rect_blur_end, PAGE_END);
    window.RegisterView(&text_end, PAGE_END);
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