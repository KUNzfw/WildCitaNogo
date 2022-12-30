#include "save.h"

#include <filesystem>
#include <fstream>

#include "board.h"
namespace save {
std::string GetPath(SLOT slot) {
  static const auto base_path = std::filesystem::path(SDL_GetBasePath());
  switch (slot) {
    case SLOT1:
      return base_path / "slot1.txt";
    case SLOT2:
      return base_path / "slot2.txt";
    case SLOT3:
      return base_path / "slot3.txt";
    default:
      return "";
  }
}
bool CheckSlotExist(SLOT slot) {
  if (!std::filesystem::is_regular_file(GetPath(slot))) {
    return false;
  }
  return true;
}
GameData GetSlotInfo(SLOT slot) {
  GameData data{};
  std::ifstream input(GetPath(slot), std::ios::binary);
  input.read(reinterpret_cast<char*>(&data.mode), sizeof(data.mode));
  input.read(reinterpret_cast<char*>(&data.bot_first), sizeof(data.bot_first));
  std::time_t cache_time{};
  input.read(reinterpret_cast<char*>(&cache_time), sizeof(cache_time));
  data.time = std::chrono::system_clock::from_time_t(cache_time);

  return data;
}
GameData GetSlotData(SLOT slot) {
  GameData data{};
  std::ifstream input(GetPath(slot), std::ios::binary);
  input.read(reinterpret_cast<char*>(&data.mode), sizeof(data.mode));
  input.read(reinterpret_cast<char*>(&data.bot_first), sizeof(data.bot_first));
  std::time_t cache_time{};
  input.read(reinterpret_cast<char*>(&cache_time), sizeof(cache_time));
  data.time = std::chrono::system_clock::from_time_t(cache_time);
  auto history_size{data.history.size()};
  input.read(reinterpret_cast<char*>(&history_size), sizeof(history_size));
  data.history.resize(history_size);
  for (auto& pos : data.history) {
    input.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
    input.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
  }
  return data;
}
void SaveToSlot(SLOT slot, GameData& data) {
  std::ofstream output(GetPath(slot), std::ios::binary);
  output.write(reinterpret_cast<char*>(&data.mode), sizeof(data.mode));
  output.write(reinterpret_cast<char*>(&data.bot_first),
               sizeof(data.bot_first));
  auto cache_time{std::chrono::system_clock::to_time_t(data.time)};
  output.write(reinterpret_cast<char*>(&cache_time), sizeof(cache_time));
  auto history_size{data.history.size()};
  output.write(reinterpret_cast<char*>(&history_size), sizeof(history_size));
  for (auto& pos : data.history) {
    output.write(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
    output.write(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
  }
}
}  // namespace save
