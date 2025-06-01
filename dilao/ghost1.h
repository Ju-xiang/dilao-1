#pragma once
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include<vector>

struct Ghost {
    int x;
    int y;
    int direction;
    int lastDirection;
};
extern int keycounts;
void initGhost1(int map[20][40], Ghost& ghost1);
void updateGhost1(int map[20][40], Ghost& ghost1, int playerX, int playerY);
int distance(int x1, int y1, int x2, int y2);