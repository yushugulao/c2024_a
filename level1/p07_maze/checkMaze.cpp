#include "main.hpp"
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

int checkMove(int newX, int newY) {
    if (maze[newY][newX] == '0') {
        return 0;  // 遇到墙壁
    } else if (maze[newY][newX] == 'E') {
        return 2;  // 到达出口
    }
    return 1;  // 可以移动
}

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

int findPoint(vector<int> point) {
    for (int i = 0; i < possiblePoints.size(); i++) {
        if (possiblePoints[i][0] == point[0] && possiblePoints[i][1] == point[1]) {
            return i;
        }
    }
    return -1;
}

void moveCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

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

