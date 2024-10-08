#include <stdio.h>
#include <conio.h>  // 用于获取键盘输入
#include <windows.h>  // 用于控制台输出及光标定位

#define WIDTH 10  // 迷宫的宽度
#define HEIGHT 10  // 迷宫的高度

// 定义迷宫地图
char maze[HEIGHT][WIDTH] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', '#', '#', '#', '#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'},
    {'#', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

// 玩家初始位置
int playerX = 1, playerY = 1;

// 移动光标位置
void moveCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 打印迷宫
void printMaze() {
    system("cls");  // 清屏
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == playerY && j == playerX) {
                printf("P");  // 玩家位置
            } else {
                printf("%c", maze[i][j]);
            }
        }
        printf("\n");
    }
}

// 检查玩家是否碰到墙壁或到达出口
int checkMove(int newX, int newY) {
    if (maze[newY][newX] == '#') {
        return 0;  // 遇到墙壁
    } else if (maze[newY][newX] == 'E') {
        return 2;  // 到达出口
    }
    return 1;  // 可以移动
}

int main() {
    int gameRunning = 1;

    printMaze();  // 初始打印迷宫

    while (gameRunning) {
        int key = _getch();  // 获取键盘输入

        // 根据用户输入移动玩家位置
        int newX = playerX;
        int newY = playerY;

        if (key == 72) {  // 向上箭头
            newY--;
        } else if (key == 80) {  // 向下箭头
            newY++;
        } else if (key == 75) {  // 向左箭头
            newX--;
        } else if (key == 77) {  // 向右箭头
            newX++;
        }

        int result = checkMove(newX, newY);
        if (result == 1) {
            playerX = newX;
            playerY = newY;
        } else if (result == 2) {
            gameRunning = 0;  // 玩家赢得游戏
        }

        printMaze();  // 重新打印迷宫
    }

    // 游戏结束时输出胜利信息
    printf("Congratulations! You won the game!\n");

    return 0;
}
