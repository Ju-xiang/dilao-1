#include "ghost1.h"
#include <vector>
#include <queue>

using namespace std;

void initGhost1(int map[20][40], Ghost& ghost1) {
    srand(time(NULL));
    do {
        ghost1.x = rand() % 40;
        ghost1.y = rand() % 20;
    } while (map[ghost1.y][ghost1.x] == 0);
    ghost1.direction = rand() % 4;
    ghost1.lastDirection = -1;
}

int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

//追逐主角的原理(21 - 63)
int Path(int map[20][40], int startX, int startY, int targetX, int targetY) {
    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    int   previousDir[20][40];
    bool visited[20][40] = { false };

    queue<pair<int, int>> q;
    q.push({ startY, startX });
    visited[startY][startX] = true;
    previousDir[startY][startX] = -1;

    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        if (x == targetX && y == targetY) {
            while (previousDir[y][x] != -1) {
                int direction = previousDir[y][x];
                int previousX = x - dx[direction];
                int previousY = y - dy[direction];
                if (previousX == startX && previousY == startY) {
                    return direction;
                }

                x = previousX;
                y = previousY;
            }
            return -1; 
        }
        for (int dir = 0; dir < 4; dir++) {
            int mx = x + dx[dir];
            int my = y + dy[dir];
            if (mx >= 0 && mx < 40 && my >= 0 && my < 20 &&map[my][mx] != 0 && !visited[my][mx]) {
                visited[my][mx] = true;
                previousDir[my][mx] = dir;
                q.push({ my, mx });
            }
        }
    }
    return rand() % 4;
}

void updateGhost1(int map[20][40], Ghost& ghost1, int playerX, int playerY) {
    static int counter = 0;
    int Frequency = (keycounts == 3) ? 4 : 8;
    if (++counter % Frequency != 0) return;
    int dist = distance(ghost1.x, ghost1.y, playerX, playerY);

    if (dist <= 6) {
        int bestDir = Path(map, ghost1.x, ghost1.y, playerX, playerY);
        int dx[4] = { 1, -1, 0, 0 };
        int dy[4] = { 0, 0, -1, 1 };

        int nx = ghost1.x + dx[bestDir];
        int ny = ghost1.y + dy[bestDir];

        if (nx >= 0 && nx < 40 && ny >= 0 && ny < 20 && map[ny][nx] != 0) {
            ghost1.x = nx;
            ghost1.y = ny;
            ghost1.lastDirection = ghost1.direction;
            ghost1.direction = bestDir;
        }
    }
   
    else {
        vector<int> validDirections;
        int dx[4] = { 1, -1, 0, 0 };
        int dy[4] = { 0, 0, -1, 1 };

     
        for (int dir = 0; dir < 4; dir++) {
            int nx = ghost1.x + dx[dir];
            int ny = ghost1.y + dy[dir];
            if (nx >= 0 && nx < 40 && ny >= 0 && ny < 20 && map[ny][nx] != 0) {
                validDirections.push_back(dir);
            }
        }

        if (!validDirections.empty()) {
            if (ghost1.lastDirection != -1) {
                int opposite = ghost1.lastDirection ^ 1;  
                validDirections.erase(remove(validDirections.begin(), validDirections.end(), opposite), validDirections.end());
                if (validDirections.empty()) validDirections.push_back(opposite);  
            }

            ghost1.direction = validDirections[rand() % validDirections.size()];
            ghost1.x += dx[ghost1.direction];
            ghost1.y += dy[ghost1.direction];
            ghost1.lastDirection = ghost1.direction;
        }
    }
}