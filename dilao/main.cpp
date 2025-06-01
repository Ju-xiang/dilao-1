#include<fstream>
#include<iostream>
#include<cmath>
#include<easyx.h>
#include<Windows.h>
#include<conio.h>
#include<chrono>
#include<mmsystem.h>
#include"ghost1.h"
#include"ghost3.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
using namespace chrono;

int keycounts = 0;
int fakeBoxCount = 0;
time_point<high_resolution_clock> gameStartTime;  // 游戏开始时间
time_point<high_resolution_clock> gameEndTime;    // 游戏结束时间
bool gameStarted = false;                         // 游戏是否已开始
bool gameEnded = false;                           // 游戏是否已结束
int totalGameTime = 0;                            // 总游戏时间（秒）

struct player
{
	int x;
	int y;
	int live;
}player;

struct GameRecord {
	int time;  // 通关时间（秒）
};

Ghost ghost[3];
Ghost3 ghost3; // 新增幽灵3

// 格式化时间显示(秒->MM:SS)
string formatTime(int seconds) {
	int minutes = seconds / 60;
	seconds = seconds % 60;
	char buffer[10];
	sprintf_s(buffer, "%02d:%02d", minutes, seconds);
	return string(buffer);
}


bool Win() {
	gameEnded = true;
	gameEndTime = high_resolution_clock::now();
	totalGameTime = duration_cast<seconds>(gameEndTime - gameStartTime).count();

	setfillcolor(RGB(0, 0, 0));
	solidrectangle(400, 225, 1200, 675);
	settextcolor(YELLOW);
	settextstyle(100, 0, "Arial");
	int titleWidth = textwidth("胜利！");
	outtextxy(825 - titleWidth / 2, 350, "胜利！");
	settextcolor(WHITE);
	settextstyle(40, 0, "Arial");
	int textWidth1 = textwidth("成功逃离地牢！");
	outtextxy(800 - textWidth1 / 2, 470, "成功逃离地牢！");
	// 显示总游戏时间
	string timeText = "总用时: " + formatTime(totalGameTime);
	int textWidthTime = textwidth(timeText.c_str());
	outtextxy(800 - textWidthTime / 2, 520, timeText.c_str());
	int textWidth2 = textwidth("按任意键返回主菜单");
	outtextxy(800 - textWidth2 / 2, 570, "按任意键返回主菜单");
	FlushBatchDraw();
	ExMessage msg;
	while (true) {
		if (peekmessage(&msg, EX_KEY)) {
			if (msg.message == WM_KEYDOWN)
				return true;
		}
	}
}
bool Lose() {
	gameEnded = true;
	gameEndTime = high_resolution_clock::now();
	totalGameTime = duration_cast<seconds>(gameEndTime - gameStartTime).count();

	setfillcolor(RGB(0, 0, 0));
	solidrectangle(400, 225, 1200, 675);
	settextcolor(RED);
	settextstyle(100, 0, "Arial");
	int titleWidth = textwidth("游戏失败！");
	outtextxy(825 - titleWidth / 2, 350, "游戏失败！");
	settextcolor(WHITE);
	settextstyle(40, 0, "Arial");
	int textWidth1 = textwidth("被幽灵抓住！");
	outtextxy(800 - textWidth1 / 2, 470, "被幽灵抓住！");
	// 显示总游戏时间
	string timeText = "总用时: " + formatTime(totalGameTime);
	int textWidthTime = textwidth(timeText.c_str());
	outtextxy(800 - textWidthTime / 2, 520, timeText.c_str());
	int textWidth2 = textwidth("按任意键返回主菜单");
	outtextxy(800 - textWidth2 / 2, 570, "按任意键返回主菜单");
	FlushBatchDraw();
	ExMessage msg;
	while (true) {
		if (peekmessage(&msg, EX_KEY)) {
			if (msg.message == WM_KEYDOWN)
				return true;
		}
	}
}
int main() {
//登陆界面
TITLE:


	int map[20][40] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,2,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
        {0,1,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0},
        {0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,0,0,1,0,3,1,1,1,1,1,1,1,0,0,1,1,1,1,0,1,0},
        {0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0},
        {0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0},
        {0,3,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0},     
		{0,1,0,1,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0},
        {0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,2,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1,0},
        {0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
        {0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0},    
		{0,1,0,1,1,1,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
		{0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0},
        {0,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0},
        {0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0},
        {1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,2,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	player.x = 0;
	player.y = 18;
	player.live = 1;

	initGhost3(map, ghost3);

	initgraph(1600, 900);
	IMAGE picture,box[2], man,wall,imageghost[3];
	loadimage(&picture, "src/jiemian.jpg", 1600, 900, true);
	loadimage(&box[0], "src/box0.jpg", 40, 40, true);
	loadimage(&box[1], "src/box1.jpg", 40, 40, true);
	loadimage(&wall, "src/wall.jpg", 40, 40, true);
	loadimage(&man, "src/man.jpg", 40, 40, true);
	loadimage(&imageghost[0], "src/ghost1.jpg", 40, 40, true);
	loadimage(&imageghost[2], "src/ghost3.png", 40, 40, true);

	BeginBatchDraw();

	auto startTime = high_resolution_clock::now();
	int gameTime = 0;

	while (true)
	{
		putimage(0, 0, &picture);
		setbkmode(TRANSPARENT);
		settextstyle(200, 0, "Elephant");
		outtextxy(500, 200, "逃离地牢");
		settextstyle(60, 0, "Elephant");
		outtextxy(700, 600, "START");
		outtextxy(700, 700, "  QUIT");

		ExMessage msg;
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (msg.x > 600 && msg.x < 1000 && msg.y > 600 && msg.y < 650) {
					cleardevice();
					gameStarted = true;
					gameEnded = false;
					gameStartTime = high_resolution_clock::now();  // 记录游戏开始时间
					goto GAME;
				}
				if (msg.x > 600 && msg.x < 1000 && msg.y > 700 && msg.y < 750) {
					cleardevice();
					return 0;
				}
			}
		}
		FlushBatchDraw();
	}
//游戏界面
GAME:
	keycounts = 0;
	fakeBoxCount = 0;
	player.live = 1;
	player.x = 0;
	player.y = 18;

	initGhost3(map, ghost3);
	initGhost1(map, ghost[0]);

	startTime = high_resolution_clock::now();

	while (true) {
		if (!gameEnded) {
			// 计算游戏时间
			auto currentTime = high_resolution_clock::now();
			long long elapsedSeconds = duration_cast<seconds>(currentTime - gameStartTime).count();

			ExMessage msg;
			if (peekmessage(&msg, EX_KEY)) {
				if (msg.message == WM_KEYDOWN) {
					int newX = player.x;
					int newY = player.y;
					switch (msg.vkcode) {
					case 'W': newY--; break;
					case 'S': newY++; break;
					case 'A': newX--; break;
					case 'D': newX++; break;
					case VK_ESCAPE: goto TITLE; break;
					}
					if (newX >= 0 && newX < 40 && newY >= 0 && newY < 20) {
						if (map[newY][newX] == 1 || map[newY][newX] == 2 || map[newY][newX] == 3) {
							if (map[newY][newX] == 2) { // 真宝箱(2)
								map[newY][newX] = 1;
								keycounts++;
							}
							else if (map[newY][newX] == 3) { // 假宝箱(3)
								map[newY][newX] = 1;
								fakeBoxCount++;
							}

							player.x = newX;
							player.y = newY;
						}
					}
				}
			}

			updateGhost3(map, ghost3, player.x, player.y, fakeBoxCount);

			const int cellSize = 40;
			const int startX = (1600 - 40 * cellSize) / 2;
			const int startY = (900 - 20 * cellSize) / 2;
			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 40; x++) {
					if (map[y][x] == 0) {
						putimage(x * cellSize + startX, y * cellSize + startY, &wall);
					}
					else if (map[y][x] == 1) {
						setfillcolor(RGB(0, 0, 0));
						solidrectangle(x * cellSize + startX, y * cellSize + startY, (x + 1) * cellSize + startX, (y + 1) * cellSize + startY);
					}
					else if (map[y][x] == 2 || (map[y][x] == 3)) {
						putimage(x * cellSize + startX, y * cellSize + startY, &box[0]);
					}
					else {
						putimage(x * cellSize + startX, y * cellSize + startY, &box[1]);
					}
				}
			}
			putimage(player.x * cellSize + startX, player.y * cellSize + startY, &man);
			updateGhost1(map, ghost[0], player.x, player.y);
			putimage(ghost[0].x * cellSize + startX, ghost[0].y * cellSize + startY, &imageghost[0]);

			if (ghost3.active && ghost3.visible) {
				putimage(ghost3.x * cellSize + startX, ghost3.y * cellSize + startY, &imageghost[2]);
			}

			settextcolor(WHITE);
			settextstyle(24, 0, "Arial");
			outtextxy(startX, startY - 50, "迷宫游戏 - 拿走三个钥匙并成功逃离");
			outtextxy(startX, startY - 30, "按ESC返回主菜单");


			// 清除之前时间显示区域
			setfillcolor(BLACK); // 使用背景色（这里假设背景色为黑色）
			solidrectangle(1600 - 100 - 20, 20, 1600 - 20, 20 + 24); // 根据实际情况调整矩形区域大小，这里假设时间显示区域宽度为100，字体高度为24
			// 在右上角显示时间（转换为int确保类型匹配）
			string timeDisplay = "时间: " + formatTime(static_cast<int>(elapsedSeconds));
			int timeWidth = textwidth(timeDisplay.c_str());
			outtextxy(1600 - timeWidth - 20, 20, timeDisplay.c_str());

			if (player.x == ghost[0].x && player.y == ghost[0].y || (ghost3.active && ghost3.visible && player.x == ghost3.x && player.y == ghost3.y)) {
				player.live = 0;
			}
			if (player.x == 39 && player.y == 1 && keycounts == 3) {
				Sleep(500);
				if (Win()) {
					cleardevice();
					goto TITLE;
				}
			}
			else if (player.live == 0) {
				Sleep(500);
				if (Lose()) {
					cleardevice();
					goto TITLE;
				}
			}
			FlushBatchDraw();
			Sleep(30);
		}
	}
}
//啊啊啊啊啊啊啊啊啊