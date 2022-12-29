#include "game.h"
namespace game {

int Judge(std::vector<Pos> history) {
  // TODO: finish the function
  if (history.size() > 10) {
    return 1;
  }
  return 0;
}
bool JudgeValid(std::vector<Pos> history, Pos p) {
  // TODO: finish the function
  return true;
}
}  // namespace game