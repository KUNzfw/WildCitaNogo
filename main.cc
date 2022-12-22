#include <iostream>
#include <random>

#include "board.h"
#include "ccgame.h"

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

enum PAGE { PAGE_MENU, PAGE_GAME, PAGE_PAUSE, PAGE_END };

int main(int argc, char **argv) {
  try {
    SDL context{CCGAME_INIT_VIDEO, CCGAME_INIT_TTF};

    Window window("Snake", WINDOW_WIDTH, WINDOW_HEIGHT);

    Font joystix28(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 28);

    window.SetBackgroundColor(0xAA, 0xAA, 0xAA, 0xFF);

    // page menu start
    RectView button1(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3,
                              WINDOW_WIDTH / 3, WINDOW_HEIGHT / 6},
                     true, SDL_Color{0x33, 0x33, 0xBB, 0xFF});
    button1.SetAlign(CCGAME_ALIGN_CENTER);

    button1.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button1.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_GAME);
          }
        });
    TextView text1("START", &joystix28, button1.GetX(), button1.GetY());
    text1.SetAlign(CCGAME_ALIGN_CENTER);

    RectView button2(SDL_Rect{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2,
                              WINDOW_WIDTH / 3, WINDOW_HEIGHT / 6},
                     true, SDL_Color{0xCC, 0x22, 0x22, 0xFF});
    button2.SetAlign(CCGAME_ALIGN_CENTER);
    button2.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button2.IsInside(x, y)) {
            window.Quit();
          }
        });

    TextView text2("QUIT", &joystix28, button2.GetX(), button2.GetY());
    text2.SetAlign(CCGAME_ALIGN_CENTER);

    window.RegisterView(&button1, PAGE_MENU);
    window.RegisterView(&text1, PAGE_MENU);
    window.RegisterView(&button2, PAGE_MENU);
    window.RegisterView(&text2, PAGE_MENU);
    // page menu end

    // page game start
    Font joystix14(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 14);
    TextView button3("PAUSE", &joystix14, 10, 10);

    Board board(120, 40);

    button3.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button3.IsInside(x, y)) {
            window.ShowGroup(PAGE_PAUSE);
          }
        });

    window.RegisterView(&button3, PAGE_GAME);
    window.RegisterView(&board, PAGE_GAME);
    // page game end

    // page pause start
    RectView rect1(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                   SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect1.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect1.SetAlpha(0xCC);

    TextView button4("CONTINUE", &joystix28, WINDOW_WIDTH / 2,
                     WINDOW_HEIGHT / 3);
    button4.SetAlign(CCGAME_ALIGN_CENTER);
    button4.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button4.IsInside(x, y)) {
            window.HideGroup(PAGE_PAUSE);
          }
        });

    TextView button5("EXIT", &joystix28, WINDOW_WIDTH / 2,
                     WINDOW_HEIGHT / 3 * 2);
    button5.SetAlign(CCGAME_ALIGN_CENTER);
    button5.SetOnMouseButtonDownListener(
        [&](Context &context, Sint32 x, Sint32 y, Uint8 button) {
          if (button5.IsInside(x, y)) {
            window.HideAllGroup();
            window.ShowGroup(PAGE_MENU);
          }
        });

    window.RegisterView(&rect1, PAGE_PAUSE);
    window.RegisterView(&button4, PAGE_PAUSE);
    window.RegisterView(&button5, PAGE_PAUSE);
    // page pause end

    // page end start
    RectView rect2(SDL_Rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, true,
                   SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
    rect2.SetBlendMode(SDL_BLENDMODE_BLEND);
    rect2.SetAlpha(0xCC);

    Font joystix20(std::string(SDL_GetBasePath()) + "assets/joystix.ttf", 20);
    TextView text3("YOU DIED!PRESS ANY KEY TO RESTART!", &joystix20,
                   WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    text3.SetAlign(CCGAME_ALIGN_CENTER);
    text3.SetOnKeydownListener([&](Context &context, SDL_Keycode keycode) {
      window.HideGroup(PAGE_END);
    });

    window.RegisterView(&rect2, PAGE_END);
    window.RegisterView(&text3, PAGE_END);
    // page end end

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