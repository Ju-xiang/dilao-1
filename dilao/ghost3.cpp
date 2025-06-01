#include "ghost3.h"

// 初始化幽灵3
void initGhost3(int map[20][40], Ghost3& ghost3) {
    ghost3.active = false;
    ghost3.visible = false;
    ghost3.moveTimer = 0;
    ghost3.moveInterval = 25; // 每秒4帧，每25帧移动一次
}

// 更新幽灵3状态
void updateGhost3(int map[20][40], Ghost3& ghost3, int playerX, int playerY, int fakeBoxCount) {
    // 检查是否需要激活幽灵3
    if (fakeBoxCount >= 2 && !ghost3.active) {
        ghost3.active = true;
        // 随机初始位置（避开墙壁）
        do {
            ghost3.x = rand() % 40;
            ghost3.y = rand() % 20;
        } while (map[ghost3.y][ghost3.x] == 0);
    }

    if (!ghost3.active) return;

    // 每25帧移动一次
    if (++ghost3.moveTimer >= ghost3.moveInterval) {
        ghost3.moveTimer = 0;

        // 计算与主角的距离，这里直接使用已有的distance函数
        int dist = distance(ghost3.x, ghost3.y, playerX, playerY);

        // 如果距离超过2格，瞬移到主角周围
        if (dist > 2) {
            int newX, newY;
            do {
                int offsetX = (rand() % 5) - 2; // -2到2
                int offsetY = (rand() % 5) - 2;
                newX = playerX + offsetX;
                newY = playerY + offsetY;
            } while (
                newX < 0 || newX >= 40 ||
                newY < 0 || newY >= 20 ||
                map[newY][newX] == 0
                );

            ghost3.x = newX;
            ghost3.y = newY;
            ghost3.visible = true;
        }
        // 否则向主角移动
        else {
            // 简单AI：尝试向主角移动
            int bestDir = -1;
            int minDist = INT_MAX;
            int dx[4] = { 1, -1, 0, 0 };
            int dy[4] = { 0, 0, -1, 1 };

            // 尝试四个方向，选择最接近主角的方向
            for (int i = 0; i < 4; i++) {
                int newX = ghost3.x + dx[i];
                int newY = ghost3.y + dy[i];

                if (newX >= 0 && newX < 40 && newY >= 0 && newY < 20 && map[newY][newX] != 0) {
                    int newDist = distance(newX, newY, playerX, playerY);
                    if (newDist < minDist) {
                        minDist = newDist;
                        bestDir = i;
                    }
                }
            }

            // 移动到最佳方向
            if (bestDir != -1) {
                ghost3.x += dx[bestDir];
                ghost3.y += dy[bestDir];
            }
        }
    }
}