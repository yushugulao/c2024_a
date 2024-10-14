///created by whf

///迷宫生成与角色移动
///水域：不可移动地块     陆地：可移动地块   围墙：迷宫边缘
///地图随机生成思路：prim算法，但是权值均相同
///选定一个紧挨围墙的初始通行点，将周围点设定为毗邻点，通过毗邻点将初始点与下一个通行点联通，毗邻点变为新生陆地，并且将下一个位置周围的点设置为毗邻点，此时若新设置的毗邻点遇到了其他通行点，
///则将该毗邻点设置为水域。1代表初始陆地，2代表通行点，3代表毗邻点，4代表新生陆地。最后所有新生陆地与通行点即为陆地，其他地块即为水域
///终点被设定为起始点所在边的对边上的一个陆地
///进度条仅供参考，不代表实际迷宫生成进度
///迷宫的长与宽都为奇数；总面积应不超过1500，否则极低的刷新率会严重影响游戏体验


#include <stdio.h>
#include <conio.h>  // 用于获取键盘输入
#include <windows.h>  // 用于控制台输出及光标定位
#include <ctime>
#include <iostream>
#include <time.h>
#include <vector>
#include <bits/streambuf_iterator.h>
using namespace std;
#define WIDTH 61  // 迷宫的宽度
#define HEIGHT 19  // 迷宫的高度

static int chosenPoint;
// 定义迷宫地图
static char maze[HEIGHT][WIDTH];
// 玩家初始位置
int playerX = 1, playerY = 1;
// 定义生成迷宫点位
static vector<vector<int> > possiblePoints;
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
    printf("\n");
    cout<<possiblePoints[chosenPoint][0]<<','<<possiblePoints[chosenPoint][1];
}

// 检查玩家是否碰到墙壁或到达出口
int checkMove(int newX, int newY) {
    if (maze[newY][newX] == '0') {
        return 0;  // 遇到墙壁
    } else if (maze[newY][newX] == 'E') {
        return 2;  // 到达出口
    }
    return 1;  // 可以移动
}

////迷宫生成算法相关函数////
//对方向的考虑顺序为上，下，左，右
//在可能点存储中找到对应点
int findPoint(vector<int> point) {
    for (int i = 0; i < possiblePoints.size(); i++) {
        if (possiblePoints[i][0] == point[0] && possiblePoints[i][1] == point[1]) {
            return i;
        }
    }
}
//检查迷宫当中是否还存在未处理的中间状态水域
bool checkMaze() {
    bool flag = true;
    for(int i=0;i<HEIGHT;i++) {
        for(int j=0;j<WIDTH;j++) {
            if (maze[i][j] == '3') {
                flag = false;
            }
        }
    }
    return flag;
}

//分别观察上，下，左，右方向未处理的原始水域
vector<int> SearchSurrounding(vector<int> point) {
    vector<int> result={0,0,0,0};
    if (maze[point[1]-1][point[0]]=='3') {
        result[0]=1;
    }
    if (maze[point[1]+1][point[0]]=='3') {
        result[1]=1;
    }
    if (maze[point[1]][point[0]-1]=='3') {
        result[2]=1;
    }
    if (maze[point[1]][point[0]+1]=='3') {
        result[3]=1;
    }
    return result;
}

//开辟通路
void CreateRoad(vector<int> point) {
    vector<int> result=SearchSurrounding(point);
    bool resultFlag=true;
    for(int i=0;i<result.size();i++) {
        if (result[i]!=0) {
            resultFlag=false;
        }
    }
    if (resultFlag) {
        int pointIndex=findPoint(point);
        possiblePoints.erase(possiblePoints.begin()+pointIndex);
    }
    int i=0;
    bool flag = true;
    vector<int> resultmirror={0,1,2,3};     // 防止一直重复判定一个方向
    while(flag || result[resultmirror[i]]!=1) {
        flag = false;
        srand((unsigned)time(NULL));
        i=rand()%resultmirror.size();
        if(result[resultmirror[i]]!=1) {
            resultmirror.erase(resultmirror.begin()+i);
            continue;
        }
        if(resultmirror[i]==0) {
            if(maze[point[1]-2][point[0]] == '1') {
                maze[point[1]-1][point[0]] = '4';
                maze[point[1]-2][point[0]] = '2';
                possiblePoints.push_back({point[0],point[1]-2});
                if(maze[point[1]-3][point[0]] != '0' && maze[point[1]-4][point[0]] != '2') {
                    maze[point[1]-3][point[0]] = '3';
                }
                //else if(maze[point[1]-4][point[0]] == '2') {
                    //maze[point[1]-3][point[0]] = '0';
                //}
                if(maze[point[1]-2][point[0]-1] != '0' && maze[point[1]-2][point[0]-2] != '2') {
                    maze[point[1]-2][point[0]-1] = '3';
                }
                //else if(maze[point[1]-2][point[0]-2] == '2') {
                    //maze[point[1]-2][point[0]-2] = '0';
                //}
                if(maze[point[1]-2][point[0]+1] != '0' && maze[point[1]-2][point[0]+2] != '2') {
                    maze[point[1]-2][point[0]+1] = '3';
                }
                //else if(maze[point[1]-2][point[0]+2] == '2') {
                    //maze[point[1]-2][point[0]+1] = '0';
                //}
            }
            else {
                maze[point[1]-1][point[0]] = '0';
            }
        }
        else if(resultmirror[i]==1) {
            if(maze[point[1]+2][point[0]] == '1') {
                maze[point[1]+1][point[0]] = '4';
                maze[point[1]+2][point[0]] = '2';
                possiblePoints.push_back({point[0],point[1]+2});
                if(maze[point[1]+3][point[0]] != '0' && maze[point[1]+4][point[0]] != '2') {
                    maze[point[1]+3][point[0]] = '3';
                }
                else if(maze[point[1]+4][point[0]] == '2') {
                    maze[point[1]+3][point[0]] = '0';
                }
                if(maze[point[1]+2][point[0]-1] != '0' && maze[point[1]+2][point[0]-2] != '2') {
                    maze[point[1]+2][point[0]-1] = '3';
                }
                else if(maze[point[1]+2][point[0]-2] == '2') {
                    maze[point[1]+2][point[0]-1] = '0';
                }
                if(maze[point[1]+2][point[0]+1] != '0' && maze[point[1]+2][point[0]+2] != '2') {
                    maze[point[1]+2][point[0]+1] = '3';
                }
                else if(maze[point[1]+2][point[0]+2] == '2') {
                    maze[point[1]+2][point[0]+1] = '0';
                }
            }
            else {
                maze[point[1]+1][point[0]] = '0';
            }
        }
        else if(resultmirror[i]==2 ) {
            if(maze[point[1]][point[0]-2] == '1') {
                maze[point[1]][point[0]-1] = '4';
                maze[point[1]][point[0]-2] = '2';
                possiblePoints.push_back({point[0]-2,point[1]});
                if(maze[point[1]][point[0]-3] != '0' && maze[point[1]][point[0]-4] != '2') {
                    maze[point[1]][point[0]-3] = '3';
                }
                else if(maze[point[1]][point[0]-4] == '2') {
                    maze[point[1]][point[0]-3] = '0';
                }
                if(maze[point[1]+1][point[0]-2] != '0' && maze[point[1]+2][point[0]-2] != '2') {
                    maze[point[1]+1][point[0]-2] = '3';
                }
                else if(maze[point[1]+2][point[0]-2] == '2') {
                    maze[point[1]+1][point[0]-2] = '0';
                }
                if(maze[point[1]-1][point[0]-2] != '0' && maze[point[1]-2][point[0]-2] != '2') {
                    maze[point[1]-1][point[0]-2] = '3';
                }
                else if(maze[point[1]-2][point[0]-2] == '2') {
                    maze[point[1]-1][point[0]-2] = '0';
                }
            }
            else {
                maze[point[1]][point[0]-1] = '0';
            }
        }
        else if(resultmirror[i]==3) {
            if(maze[point[1]][point[0]+2] == '1') {
                maze[point[1]][point[0]+1] = '4';
                maze[point[1]][point[0]+2] = '2';
                possiblePoints.push_back({point[0]+2,point[1]});
                if(maze[point[1]][point[0]+3] != '0' && maze[point[1]][point[0]+4] != '2') {
                    maze[point[1]][point[0]+3] = '3';
                }
                else if(maze[point[1]][point[0]+4] == '2') {
                    maze[point[1]][point[0]+3] = '0';
                }
                if(maze[point[1]+1][point[0]+2] != '0' && maze[point[1]+2][point[0]+2] != '2') {
                    maze[point[1]+1][point[0]+2] = '3';
                }
                else if(maze[point[1]+2][point[0]+2] == '2') {
                    maze[point[1]+1][point[0]+2] = '0';
                }
                if(maze[point[1]-1][point[0]+2] != '0' && maze[point[1]-2][point[0]+2] != '2') {
                    maze[point[1]-1][point[0]+2] = '3';
                }
                else if(maze[point[1]-2][point[0]+2] != '2') {
                    maze[point[1]-1][point[0]+2] = '0';
                }
            }
            else {
                maze[point[1]][point[0]+1] = '0';
            }
        }
    }
}


///

int main() {
    int gameRunning = 1;
    //先生成一个未打开通路的迷宫雏形
    for(int i=0;i<HEIGHT;i++) {
        for(int j=0;j<WIDTH;j++) {
            maze[i][j] = '#';  //初始水域
            if (i==0 || i==HEIGHT-1 || j==0 || j==WIDTH-1) {
                maze[i][j] = '0';  //外围地块
            }
            if (i%2==1 && j%2==1) {
                maze[i][j] = '1';  //初始陆地
            }
        }
    }


    int startX = 1, startY = 1;
    //在毗邻围墙的起始陆地找到起始点
    srand((unsigned)time(NULL)+23);
    int whichside=rand()%4;
    while (true) {
        if (whichside==0) {
            startY = 1;
            srand((unsigned)time(NULL));
            startX = rand()%(WIDTH-4)+2;
        }
        else if (whichside==1) {
            startY = HEIGHT-2;
            srand((unsigned)time(NULL));
            startX = rand()%(WIDTH-4)+2;
        }
        else if (whichside==3) {
            startX = WIDTH-2;
            srand((unsigned)time(NULL));
            startY = rand()%(HEIGHT-4)+2;
        }
        else {
            startX = 1;
            srand((unsigned)time(NULL));
            startY = rand()%(HEIGHT-4)+2;
        }
        if(maze[startY][startX]=='1')  {
            maze[startY][startX] = '2';
            break;
        }
    }
    vector<int> Startpoint = {startX, startY};
    playerX = Startpoint[0];
    playerY = Startpoint[1];
    //从起始点开始开辟路径
    possiblePoints.push_back(Startpoint);
    if (maze[playerY-1][playerX]!='0') {
        maze[playerY-1][playerX] = '3';
    }
    if (maze[playerY+1][playerX]!='0') {
        maze[playerY+1][playerX] = '3';
    }
    if (maze[playerY][playerX-1]!='0') {
        maze[playerY][playerX-1] = '3';
    }
    if (maze[playerY][playerX+1]!='0') {
        maze[playerY][playerX+1] = '3';
    }

    int passtimes=HEIGHT*WIDTH;
    int countTimes=0;
    while(true) {
        if (passtimes>0 || !checkMaze()) {
            passtimes--;
            srand((unsigned)time(NULL));
            chosenPoint = rand()%possiblePoints.size();
            vector<int> result=SearchSurrounding(possiblePoints[chosenPoint]);
            bool resultFlag=true;
            for(int i=0;i<result.size();i++) {
                if (result[i]!=0) {
                    resultFlag=false;
                }
            }
            if (resultFlag) {
                possiblePoints.erase(possiblePoints.begin()+chosenPoint);
                continue;
            }
            CreateRoad(possiblePoints[chosenPoint]);
            countTimes++;
            //进度条
            if (countTimes%20==0) {
                system("cls");
                float CompleteRate=(float)countTimes/(HEIGHT*WIDTH)/3*100;
                cout<<CompleteRate<<"%"<<" "<<"Maze Generating,please wait a moment!";
            }
        }
        else {
            break;
        }
    }



    int endX = 1, endY = 1;
    //生成迷宫的终点
    while (true) {
        if (whichside==1) {
            endY = 1;
            srand((unsigned)time(NULL));
            endX = rand()%(WIDTH-4)+2;
        }
        else if (whichside==0) {
            endY = HEIGHT-2;
            srand((unsigned)time(NULL));
            endX = rand()%(WIDTH-4)+2;
        }
        else if (whichside==2) {
            endX = WIDTH-2;
            srand((unsigned)time(NULL));
            endY = rand()%(HEIGHT-4)+2;
        }
        else {
            endX = 1;
            srand((unsigned)time(NULL));
            endY = rand()%(HEIGHT-4)+2;
        }
        if(maze[endY][endX]=='2' or maze[endY][endX]=='4')  {
            maze[endY][endX] = 'E';
            break;
        }
    }

    //更改迷宫的格式使得面向用户友好
    for(int i=0;i<HEIGHT;i++) {
        for(int j=0;j<WIDTH;j++) {
            if (maze[i][j] == '2' || maze[i][j] == '4') {
                maze[i][j] = ' ';
            }
            else if (maze[i][j] == '#') {
                maze[i][j] = '0';
            }
        }
    }


    // 初始打印迷宫
    printMaze();

    while (gameRunning) {
        int key = _getch();  // 获取键盘输入

        // 根据用户输入移动玩家位置
        int newX = playerX;
        int newY = playerY;

        if (key == 72) {  // 向上箭头
            newY--;
        }
        else if (key == 80) {  // 向下箭头
            newY++;
        }
        else if (key == 75) {  // 向左箭头
            newX--;
        }
        else if (key == 77) {  // 向右箭头
            newX++;
        }

        int result = checkMove(newX, newY);
        if (result == 1) {
            playerX = newX;
            playerY = newY;
        }
        else if (result == 2) {
            gameRunning = 0;  // klee逃出禁闭室
        }

        printMaze();  // 重新打印迷宫
    }

    // 游戏结束时输出胜利信息
    printf("Congratulations! You won the game! LETS GO FRY FISH WITH KLEE!\n");
    system("pause");
    return 0;
}
