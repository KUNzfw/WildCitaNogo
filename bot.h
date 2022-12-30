#ifndef NOGO_BOT_h_
#define NOGO_BOT_h_

#include <string>

#include "ccgame.h"

namespace bot {
int RunEasyBot([[maybe_unused]] void* data);
int RunHardBot([[maybe_unused]] void* data);
bool IsEnd();
void PrepareRun();
void SetInput(std::string input);
std::string GetOutput();
}  // namespace bot

#endif  // NOGO_BOT_h_
