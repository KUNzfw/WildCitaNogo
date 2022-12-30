#include "game.h"
namespace game {
int chessBoard[9][9]{};          // ����
const int cx[] = {-1, 0, 1, 0};  // �ƶ�����
const int cy[] = {0, -1, 0, 1};
bool dfs_air_visit[9][9]{};  // ������

bool inBorder(int x, int y) { return x >= 0 && y >= 0 && x < 9 && y < 9; }

// true: has air
bool dfs_air(int fx, int fy) {
  dfs_air_visit[fx][fy] = true;
  bool flag = false;
  for (int dir = 0; dir < 4; dir++) {
    int dx = fx + cx[dir], dy = fy + cy[dir];
    if (inBorder(dx, dy)) {
      if (chessBoard[dx][dy] == 0) flag = true;
      if (chessBoard[dx][dy] == chessBoard[fx][fy] && !dfs_air_visit[dx][dy]) {
        if (dfs_air(dx, dy)) {
          flag = true;
        }
      }
    }
  }
  return flag;
}

bool judge_next(Pos p, bool black) {
  chessBoard[p.x][p.y] = black ? -1 : 1;
  bool result = true;
  int fx = p.x, fy = p.y;
  for (int dir = 0; dir < 4; dir++) {
    int dx = fx + cx[dir], dy = fy + cy[dir];
    if (inBorder(dx, dy) && chessBoard[dx][dy]) {
      memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
      result = result && dfs_air(dx, dy);
    }
  }
  memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
  result = result && dfs_air(fx, fy);
  return result;
}

int Judge(std::vector<Pos> history) {
  memset(chessBoard, 0, sizeof(chessBoard));
  int step = history.size();
  int lastStep = step % 2 ? -1 : 1;
  for (int i = 0; i < step; i++) {
    chessBoard[history[i].x][history[i].y] = i % 2 ? 1 : -1;
  }
  bool result = true;
  int fx = history[step - 1].x, fy = history[step - 1].y;
  for (int dir = 0; dir < 4; dir++) {
    int dx = fx + cx[dir], dy = fy + cy[dir];
    if (inBorder(dx, dy) && chessBoard[dx][dy]) {
      memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
      result = result && dfs_air(dx, dy);
    }
  }
  memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
  result = result && dfs_air(fx, fy);
  if (lastStep == -1 && !result) {
    return 1;
  } else if (lastStep == 1 && !result) {
    return -1;
  }
  bool black = (lastStep == 1);
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!chessBoard[i][j]) {
        Pos p = {i, j};
        if (judge_next(p, black)) {
          return 0;
        }
      }
    }
  }
  if (black) {
    return 1;
  } else {
    return -1;
  }
  // �����û�����򷵻�0������ʤ����-1������ʤ����1
  // ע�⣬��ĳһ�����ӿ��µ�ʱ��Ҳ�ж�Ϊ������������һ����ʤ
}

bool JudgeValid(std::vector<Pos> history, Pos p) {
  memset(chessBoard, 0, sizeof(chessBoard));
  int step = history.size();
  bool black = step % 2 ? false : true;
  for (int i = 0; i < step; i++) {
    chessBoard[history[i].x][history[i].y] = i % 2 ? 1 : -1;
  }
  if (!judge_next(p, black)) {
    return false;
  }
  return true;
}
}  // namespace game