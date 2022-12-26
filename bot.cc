#include "bot.h"

#include <utility>

#include "thirdparty/cpp-subprocess/subprocess.hpp"
using namespace subprocess;
namespace bot {
SDL_SpinLock data_input_lock{0};
std::string data_input{};
SDL_SpinLock data_output_lock{0};
std::string data_output{};
SDL_SpinLock is_end_lock{0};
bool is_end{};
int RunHardBot([[maybe_unused]] void *data) {
#ifdef _WIN32
  std::string path = SDL_GetBasePath() + std::string("bot/cita/Cita.exe");
#else
  std::string path = SDL_GetBasePath() + std::string("bot/cita/Cita");
#endif

  auto p = Popen({path}, output{PIPE}, input{PIPE});
  SDL_AtomicLock(&data_input_lock);
  p.send(data_input);
  SDL_AtomicUnlock(&data_input_lock);

  auto res = p.communicate();
  SDL_AtomicLock(&data_output_lock);
  data_output = res.first.buf.data();
  SDL_AtomicUnlock(&data_output_lock);

  SDL_AtomicLock(&is_end_lock);
  is_end = true;
  SDL_AtomicUnlock(&is_end_lock);
  return 0;
}
bool IsEnd() {
  SDL_AtomicLock(&is_end_lock);
  bool result = is_end;
  SDL_AtomicUnlock(&is_end_lock);
  return result;
}
void PrepareRun() {
  SDL_AtomicLock(&is_end_lock);
  is_end = false;
  SDL_AtomicUnlock(&is_end_lock);
}
std::string GetOutput() {
  SDL_AtomicLock(&data_output_lock);
  std::string res = data_output;
  SDL_AtomicUnlock(&data_output_lock);
  return res;
}
void SetInput(std::string input) {
  SDL_AtomicLock(&data_input_lock);
  data_input = std::move(input);
  SDL_AtomicUnlock(&data_input_lock);
}
}  // namespace bot
