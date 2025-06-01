#include "ghost3.h"

// ��ʼ������3
void initGhost3(int map[20][40], Ghost3& ghost3) {
    ghost3.active = false;
    ghost3.visible = false;
    ghost3.moveTimer = 0;
    ghost3.moveInterval = 25; // ÿ��4֡��ÿ25֡�ƶ�һ��
}

// ��������3״̬
void updateGhost3(int map[20][40], Ghost3& ghost3, int playerX, int playerY, int fakeBoxCount) {
    // ����Ƿ���Ҫ��������3
    if (fakeBoxCount >= 2 && !ghost3.active) {
        ghost3.active = true;
        // �����ʼλ�ã��ܿ�ǽ�ڣ�
        do {
            ghost3.x = rand() % 40;
            ghost3.y = rand() % 20;
        } while (map[ghost3.y][ghost3.x] == 0);
    }

    if (!ghost3.active) return;

    // ÿ25֡�ƶ�һ��
    if (++ghost3.moveTimer >= ghost3.moveInterval) {
        ghost3.moveTimer = 0;

        // ���������ǵľ��룬����ֱ��ʹ�����е�distance����
        int dist = distance(ghost3.x, ghost3.y, playerX, playerY);

        // ������볬��2��˲�Ƶ�������Χ
        if (dist > 2) {
            int newX, newY;
            do {
                int offsetX = (rand() % 5) - 2; // -2��2
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
        // �����������ƶ�
        else {
            // ��AI�������������ƶ�
            int bestDir = -1;
            int minDist = INT_MAX;
            int dx[4] = { 1, -1, 0, 0 };
            int dy[4] = { 0, 0, -1, 1 };

            // �����ĸ�����ѡ����ӽ����ǵķ���
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

            // �ƶ�����ѷ���
            if (bestDir != -1) {
                ghost3.x += dx[bestDir];
                ghost3.y += dy[bestDir];
            }
        }
    }
}