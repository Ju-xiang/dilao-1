#pragma once
#ifndef GHOST3_H
#define GHOST3_H

#include <cstdlib>
#include <ctime>
// 假设distance函数声明在ghost1.h中，这里只需包含头文件
#include "ghost1.h" 

// 幽灵3的结构体定义
struct Ghost3 {
    int x, y;           // 位置坐标
    bool active;        // 是否激活（出现）
    bool visible;       // 是否可见
    int moveTimer;      // 移动计时器
    int moveInterval;   // 移动间隔（帧）
};

// 初始化幽灵3
void initGhost3(int map[20][40], Ghost3& ghost3);

// 更新幽灵3状态
void updateGhost3(int map[20][40], Ghost3& ghost3, int playerX, int playerY, int fakeBoxCount);

#endif // GHOST3_H