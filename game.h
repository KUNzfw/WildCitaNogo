#ifndef NOGO_GAME_h_
#define NOGO_GAME_h_
#include "board.h"
namespace game {
// 传入走棋历史，第一手永远是黑棋，0<=x,y<9
// 如果还没结束则返回0，黑棋胜返回-1，白棋胜返回1
// 注意，当某一方无子可下的时候也判断为结束并返回另一方获胜
int Judge(std::vector<Pos> history);

// 判断在history的基础上的下一步行棋p是否会导致己方立即失败
// 即不考虑下完这步棋后对面没法下的情况
bool JudgeValid(std::vector<Pos> history, Pos p);
}  // namespace game
#endif  // NOGO_GAME_h_
