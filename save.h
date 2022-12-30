#ifndef NOGO_SAVE_h_
#define NOGO_SAVE_h_
#include <chrono>
#include <string>
#include <vector>

struct Pos;
enum GAME_MODE : int;

namespace save {
enum SLOT { SLOT1, SLOT2, SLOT3 };
struct GameData {
  GAME_MODE mode;
  bool bot_first;
  std::chrono::time_point<std::chrono::system_clock> time;
  std::vector<Pos> history;
};
std::string GetPath(SLOT slot);
bool CheckSlotExist(SLOT slot);
GameData GetSlotInfo(SLOT slot);
GameData GetSlotData(SLOT slot);
void SaveToSlot(SLOT slot, GameData& data);
}  // namespace save
#endif  // NOGO_SAVE_H
