///created by whf
///

///迷宫生成与角色移动
///水域：不可移动地块     陆地：可移动地块   围墙：迷宫边缘
///地图随机生成思路：prim算法，但是权值均相同
///选定一个紧挨围墙的初始通行点，将周围点设定为毗邻点，通过毗邻点将初始点与下一个通行点联通，毗邻点变为新生陆地，并且将下一个位置周围的点设置为毗邻点，此时若新设置的毗邻点遇到了其他通行点，
///则将该毗邻点设置为水域。1代表初始陆地，2代表通行点，3代表毗邻点，4代表新生陆地。最后所有新生陆地与通行点即为陆地，其他地块即为水域
///终点被设定为起始点所在边的对边上的一个陆地
///进度条仅供参考，不代表实际迷宫生成进度
///迷宫的长与宽都为奇数；总面积应不超过1500，否则极低的刷新率会严重影响游戏体验

#include "main.hpp"

int chosenPoint;
// 定义迷宫地图
char maze[HEIGHT][WIDTH];
// 玩家初始位置
int playerX = 1, playerY = 1;
// 定义生成迷宫点位
vector<vector<int> > possiblePoints;

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