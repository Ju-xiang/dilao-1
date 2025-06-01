#pragma once
#ifndef GHOST3_H
#define GHOST3_H

#include <cstdlib>
#include <ctime>
// ����distance����������ghost1.h�У�����ֻ�����ͷ�ļ�
#include "ghost1.h" 

// ����3�Ľṹ�嶨��
struct Ghost3 {
    int x, y;           // λ������
    bool active;        // �Ƿ񼤻���֣�
    bool visible;       // �Ƿ�ɼ�
    int moveTimer;      // �ƶ���ʱ��
    int moveInterval;   // �ƶ������֡��
};

// ��ʼ������3
void initGhost3(int map[20][40], Ghost3& ghost3);

// ��������3״̬
void updateGhost3(int map[20][40], Ghost3& ghost3, int playerX, int playerY, int fakeBoxCount);

#endif // GHOST3_H