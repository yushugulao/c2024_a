#ifndef MAZE_H
#define MAZE_H

#include "stdio.h"
#include "conio.h"  // 用于获取键盘输入
#include "windows.h"  // 用于控制台输出及光标定位
#include "ctime"
#include "iostream"
#include <time.h>
#include "vector"
#include <bits/streambuf_iterator.h>
using namespace std;
#define WIDTH 61  // 迷宫的宽度
#define HEIGHT 19  // 迷宫的高度
extern int chosenPoint;
// 定义迷宫地图
extern char maze[HEIGHT][WIDTH];
// 玩家初始位置
extern int playerX, playerY;
// 定义生成迷宫点位
extern vector<vector<int> > possiblePoints;
// 移动光标
void moveCursor(int x, int y);
// 打印迷宫
void printMaze();
// 检查玩家是否碰到墙壁或到达出口
int checkMove(int newX, int newY);
//对方向的考虑顺序为上，下，左，右
//在可能点存储中找到对应点
int findPoint(vector<int> point);
//检查迷宫当中是否还存在未处理的中间状态水域
bool checkMaze();
//分别观察上，下，左，右方向未处理的原始水域
vector<int> SearchSurrounding(vector<int> point);
//开辟通路
void CreateRoad(vector<int> point);
#endif