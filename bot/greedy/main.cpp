// by 李静雯
#include <iostream>
#include <cstring>
using namespace std;
struct Pos {
  int x, y;
};
int chessBoard[9][9]{};
const int cx[] = {-1, 0, 1, 0};
const int cy[] = {0, -1, 0, 1};
bool dfs_air_visit[9][9]{};

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
  chessBoard[p.x][p.y] = 0;  //
  return result;
}

int chess_value(bool black) {
  int cnt = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!chessBoard[i][j]) {
        Pos p = {i, j};
        if (judge_next(p, true)) cnt++;
        if (judge_next(p, false)) cnt--;
      }
    }
  }
  if (!black) {
    cnt *= -1;
  }
  return cnt;
}

int main() {
  int value[9][9]{};
  int n, x, y;
  cin >> n;
  bool black = false;
  for (int i = 0; i < 2 * n - 1; i++) {
    cin >> x >> y;
    if (x == -1) {
      black = true;
    } else {
      if (black) {
        chessBoard[x][y] = i % 2 ? -1 : 1;
      }
      if (!black) {
        chessBoard[x][y] = i % 2 ? 1 : -1;
      }
    }
  }
  int maxValue = -100;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (chessBoard[i][j]) {
        value[i][j] = -100;
      } else {
        chessBoard[i][j] = black ? -1 : 1;
        value[i][j] = chess_value(black);
        chessBoard[i][j] = 0;
        if (value[i][j] > maxValue) {
          maxValue = value[i][j];
        }
      }
    }
  }
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (value[i][j] == maxValue) {
        cout << i << ' ' << j;
        exit(0);
      }
    }
  }
}