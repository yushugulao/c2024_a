#ifndef AIWORKER_H
#define AIWORKER_H

#include <QObject>
#include <QPoint>
#include <vector>
#include <QVector>
#include <limits>
#include <chrono>
#include <mutex>
#include <atomic>
#include <QtConcurrent>
#include <future>

const int BOARD_SIZE = 15;

class AIWorker : public QObject
{
    Q_OBJECT
public:
    explicit AIWorker(QObject *parent = nullptr);
    ~AIWorker();

    void setBoard(const int board[BOARD_SIZE][BOARD_SIZE]);
    void setAiColor(int color);

    // AI 行棋入口
    QPoint aiMove();

signals:
    void resultReady(int x, int y);

public slots:
    void process(); // 主处理函数

private:
    // 原有成员变量
    int board[BOARD_SIZE][BOARD_SIZE];
    int aiColor;
    enum { Empty = 0, Black = 1, White = 2 };

    // 与迭代加深及搜索相关的成员
    int moveCount;                        // 当前已有步数
    QPoint  interativeDeepeningMove();
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds timeLimit{100000};

    // 原有开局库
    QVector<QVector<QPoint>> openingBook;

    // 评估/搜索相关函数
    int evaluate(int state[BOARD_SIZE][BOARD_SIZE], int player);
    int evaluatePlayer(int state[BOARD_SIZE][BOARD_SIZE], int player);
    int evaluatePoint(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player);
    int evaluatePattern(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, int player);
    int evaluateThreat(int state[BOARD_SIZE][BOARD_SIZE], int player);

    bool checkWin(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player);
    bool checkThree(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player);
    bool checkFour(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player);
    bool checkAttackMove(int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player, int depthNow);
    int countDoubleLiveThree(int state[BOARD_SIZE][BOARD_SIZE], int player);
    int countDoubleBlockedFour(int state[BOARD_SIZE][BOARD_SIZE], int player);
    int countLiveThreePlusBlockedFour(int state[BOARD_SIZE][BOARD_SIZE], int player);
    int countConsecutive(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, int player);

    // Minimax 相关
    int minimax(int state[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta,
                bool maximizingPlayer, int lastMoveX, int lastMoveY);

    // 生成候选走法
    std::vector<std::pair<int, int>> generateCandidateMoves(int state[BOARD_SIZE][BOARD_SIZE], int player, int depthNow);
    QPoint normalMove();

    // 开局库
    void initOpeningBook();

    QPoint findImmediateThreat();
    QPoint findImmediateWin();
    QVector<QPoint> applySymmetry(const QVector<QPoint>& opening, int symmetryType);

    // 并行化相关成员
    void copyBoard(const int source[BOARD_SIZE][BOARD_SIZE], int dest[BOARD_SIZE][BOARD_SIZE]);

    // 深度初始设置
    int depthComplete = 6;
    int depthKillSearch = 11;
};

#endif // AIWORKER_H
