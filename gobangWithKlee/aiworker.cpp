
//aiworker.cpp

//aiworker.cpp
#include "aiworker.h"
#include <algorithm>
#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <cmath>

AIWorker::AIWorker(QObject *parent) : QObject(parent), moveCount(0)
{
    initOpeningBook();
}

AIWorker::~AIWorker()
{
}

void AIWorker::setBoard(const int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            this->board[i][j] = board[i][j];
}

void AIWorker::setAiColor(int color)
{
    aiColor = color;
}

void AIWorker::process()
{
    // 进行 AI 计算
    QPoint aiMovePoint = aiMove();

    // 发射结果信号
    emit resultReady(aiMovePoint.x(), aiMovePoint.y());
}

// 定义棋型的权重
const int FIVE = 10000000;          // 连五
const int FOUR = 100000;           // 活四
const int BLOCKED_FOUR = 1500;    // 眠四
const int LIVE_THREE = 1000;       // 活三
const int SLEEP_THREE = 150;       // 眠三
const int LIVE_TWO = 100;           // 活二
const int SLEEP_TWO = 15;          // 眠二
const int DOUBLE_LIVE_THREE = 50000;           // 双活三
const int DOUBLE_BLOCKED_FOUR = 100000;         // 双冲四
const int FOUR_PLUS_LIVE_THREE = 100000;        // 冲四 + 活三

void AIWorker::initOpeningBook()
{
    // 其实没啥用，对手不可能都按这个走
    QVector<QVector<QPoint>> baseOpenings = {
        {
         QPoint(7, 7),  // 中心点
         QPoint(7, 8),
         QPoint(8, 7),
         QPoint(6, 6)
        },
        {
         QPoint(7, 7),
         QPoint(7, 6),
         QPoint(8, 6),
         QPoint(8, 8)
        },
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(8, 8),
         QPoint(5, 5)
        },
        {
         QPoint(7, 7),
         QPoint(8, 6),
         QPoint(9, 5),
         QPoint(6, 8)
        },
        {
         QPoint(7, 7),
         QPoint(6, 7),
         QPoint(5, 8),
         QPoint(8, 8)
        },
        {
         QPoint(7, 7),
         QPoint(7, 8),
         QPoint(7, 6),
         QPoint(7, 9)
        },
        {
        QPoint(7, 7),
        QPoint(7, 6),
        QPoint(6, 7),
        QPoint(8, 7),
        QPoint(7, 8)},
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(8, 8),
         QPoint(6, 8),
         QPoint(8, 6)
        },
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(8, 8),
         QPoint(6, 8),
         QPoint(8, 6)
        },
        {
         QPoint(7, 7),
         QPoint(8, 7),
         QPoint(9, 7),
         QPoint(8, 8)
        },
        {
         QPoint(7, 7),
         QPoint(6, 7),
         QPoint(8, 7),
         QPoint(7, 8),
         QPoint(7, 6)
        },
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(8, 8),
         QPoint(5, 5),
         QPoint(9, 9)
},
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(6, 8),
         QPoint(8, 6),
         QPoint(8, 8)
},
        {
         QPoint(7, 7),
         QPoint(6, 6),
         QPoint(8, 7),
         QPoint(6, 8),
         QPoint(8, 9)
},
        {
         QPoint(7, 7),
         QPoint(6, 7),
         QPoint(8, 7),
         QPoint(7, 6),
         QPoint(7, 8)
}
    };

    for (const auto& opening : baseOpenings) {
        openingBook.append(opening);
        // 旋转90度
        openingBook.append(applySymmetry(opening, 1));
        // 旋转180度
        openingBook.append(applySymmetry(opening, 2));
        // 旋转270度
        openingBook.append(applySymmetry(opening, 3));
        // 镜像
        openingBook.append(applySymmetry(opening, 4));
        // 镜像转90度
        openingBook.append(applySymmetry(opening, 5));
        // 镜像180度
        openingBook.append(applySymmetry(opening, 6));
        // 镜像 270度
        openingBook.append(applySymmetry(opening, 7));
    }
}

QVector<QPoint> AIWorker::applySymmetry(const QVector<QPoint>& opening, int symmetryType)
{
    QVector<QPoint> transformed;
    for (const auto& point : opening) {
        int x = point.x();
        int y = point.y();
        switch (symmetryType) {
        case 1: // 旋转90度
            transformed.append(QPoint(BOARD_SIZE - 1 - y, x));
            break;
        case 2: // 旋转180度
            transformed.append(QPoint(BOARD_SIZE - 1 - x, BOARD_SIZE - 1 - y));
            break;
        case 3: // 旋转270度
            transformed.append(QPoint(y, BOARD_SIZE - 1 - x));
            break;
        case 4: // 镜像
            transformed.append(QPoint(x, BOARD_SIZE - 1 - y));
            break;
        case 5: // 镜像 + 旋转90度
            transformed.append(QPoint(BOARD_SIZE - 1 - y, x));
            break;
        case 6: // 镜像 + 旋转180度
            transformed.append(QPoint(BOARD_SIZE - 1 - x, y));
            break;
        case 7: // 镜像 + 旋转270度
            transformed.append(QPoint(y, BOARD_SIZE - 1 - x));
            break;
        default: // 不变
            transformed.append(point);
            break;
        }
    }
    return transformed;
}

QPoint AIWorker::aiMove()
{
    //取胜
    QPoint winMove = findImmediateWin();
    if (winMove.x() != -1 && winMove.y() != -1){
        return winMove;
    }

    //封堵
    QPoint blockingMove = findImmediateThreat();
    if (blockingMove.x() != -1 && blockingMove.y() != -1) {
        return blockingMove;
    }

    // 已下子数
    int totalMoves = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != Empty) {
                totalMoves++;
            }
        }
    }

    // 开局库：
    if (totalMoves <3) {
        for (const auto &opening : openingBook) {
            if (moveCount < opening.size()) {
                QPoint tmp = opening[moveCount];
                if (board[tmp.x()][tmp.y()] == Empty) {
                    moveCount++;
                    return tmp;
                }
            }
        }
    }

    moveCount++;

    //auto result = iterativeDeepeningSearch(board);
    //return result.second;
    //return normalMove();
    return interativeDeepeningMove();
}



QPoint AIWorker::findImmediateThreat()
{
    int opponent = (aiColor == White) ? Black : White;
    QVector<QPoint> threatMoves;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Empty) {
                board[i][j] = opponent;
                if (checkWin(board, i, j, opponent)) {
                    threatMoves.append(QPoint(i, j));
                }
                board[i][j] = Empty;
            }
        }
    }

    if (!threatMoves.isEmpty()) {
        return threatMoves.first();
    }

    return QPoint(-1, -1); // 无威胁
}

QPoint AIWorker::findImmediateWin()
{
    QVector<QPoint> winMoves;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Empty) {
                board[i][j] = aiColor;
                if (checkWin(board, i, j, aiColor)) {
                    winMoves.append(QPoint(i, j));
                }
                board[i][j] = Empty;
            }
        }
    }

    if (!winMoves.isEmpty()) {
        return winMoves.first();
    }
    return QPoint(-1, -1); // 赢不了
}

void AIWorker::copyBoard(const int source[BOARD_SIZE][BOARD_SIZE], int dest[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            dest[i][j] = source[i][j];
        }
    }
}


/*   没用了下面重写了
std::pair<int, QPoint> AIWorker::iterativeDeepeningSearch(int state[BOARD_SIZE][BOARD_SIZE])
{
    QPoint bestMove(-1, -1);
    int bestScore = std::numeric_limits<int>::min();

    auto start = std::chrono::steady_clock::now();

    for (int depth = 1; depth <= maxDepth; depth++) {
        int currentScore = std::numeric_limits<int>::min();
        QPoint currentBest(-1, -1);

        int tempBoard[BOARD_SIZE][BOARD_SIZE];
        copyBoard(state, tempBoard);
        auto moves = generateCandidateMoves(tempBoard,aiColor);

        // 限制并行化的走法数量
        int maxParallelMoves = 40;
        if (moves.size() > maxParallelMoves) {
            moves.resize(maxParallelMoves);
        }

        // 重置并行化结果
        bestScoreParallel = std::numeric_limits<int>::min();
        bestMoveParallel = QPoint(-1, -1);

        // 并行处理每个走法
        QFuture<void> future = QtConcurrent::map(moves, [&](std::pair<int, int> move) {
            int x = move.first;
            int y = move.second;

            // 复制棋盘
            int boardCopy[BOARD_SIZE][BOARD_SIZE];
            copyBoard(state, boardCopy);

            // 放置AI的棋子
            boardCopy[x][y] = aiColor;

            // 计算得分
            int score = minimax(boardCopy, depth - 1,std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),false, x, y);

            std::lock_guard<std::mutex> lock(bestMoveMutexParallel);
            if (score > bestScoreParallel) {
                bestScoreParallel = score;
                bestMoveParallel = QPoint(x, y);
            }
        });

        future.waitForFinished();

        if (bestScoreParallel > bestScore) {
            bestScore = bestScoreParallel;
            bestMove = bestMoveParallel;
        }

        if (bestScore >= FIVE) {
            break;
        }

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start) > timeLimit) {
            break;
        }
    }

    return {bestScore, bestMove};
}*/

int AIWorker::evaluate(int state[BOARD_SIZE][BOARD_SIZE], int player)
{
    int aiScore = evaluatePlayer(state, aiColor);
    int opponent = (aiColor == White) ? Black : White;
    int opponentScore = evaluatePlayer(state, opponent);
    int totalScore = aiScore - opponentScore ;
    return totalScore;
}

int AIWorker::evaluatePlayer(int state[BOARD_SIZE][BOARD_SIZE], int player)
{
    int score = 0;

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        for (int y = 0; y < BOARD_SIZE; ++y)
        {
            if (state[x][y] == player)
            {
                // 评估己方棋型
                score += evaluatePoint(state, x, y, player);
            }
        }
    }

    // 检测并评分双活三
    int doubleLiveThree = countDoubleLiveThree(state, player);
    if (doubleLiveThree >= 2)
    {
        score += DOUBLE_LIVE_THREE;
    }

    // 检测并评分双冲四
    int doubleBlockedFour = countDoubleBlockedFour(state, player);
    if (doubleBlockedFour >= 2)
    {
        score += DOUBLE_BLOCKED_FOUR;
    }

    // 检测并评分冲四 + 活三
    int fourPlusThree = countLiveThreePlusBlockedFour(state, player);
    if (fourPlusThree >= 1)
    {
        score += FOUR_PLUS_LIVE_THREE;
    }

    return score;
}

int AIWorker::evaluatePoint(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
    int score = 0;

    // 对每个方向进行检查
    for (int dir = 0; dir < 4; ++dir)
    {
        int dx = 0, dy = 0;
        switch (dir)
        {
        case 0: dx = 1; dy = 0; break;
        case 1: dx = 0; dy = 1; break;
        case 2: dx = 1; dy = 1; break;
        case 3: dx = 1; dy = -1; break;
        }
        int patternType = evaluatePattern(state, x, y, dx, dy, player);
        switch (patternType)
        {
        case 6:
            score += FIVE;
            break;
        case 5:
            score += FOUR;
            break;
        case 4:
            score += BLOCKED_FOUR;
            break;
        case 3:
            score += LIVE_THREE;
            break;
        case 2:
            score += LIVE_TWO;
            break;
        case 1:
            score +=SLEEP_TWO;
        default:
            break;
        }
    }

    return score;
}

int AIWorker::evaluatePattern(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, int player)
{
    int consecutive = 1; // 包含当前位置
    int openEnds = 0;
    bool jumpThreeFlag = false;
    int jumpFourCon = 1;

    //对跳三进行专门的检测
    if (x+4*dx >=0 && x+4*dx<BOARD_SIZE && x-dx >=0 && x-dx<BOARD_SIZE && y+4*dy>=0 && y+4*dy<BOARD_SIZE && y-dy>=0 && y-dy<BOARD_SIZE){
        if((state[x+4*dx][y+4*dy] == Empty && state[x+3*dx][y+3*dy] == player && state[x+2*dx][y+2*dy] == player && state[x+dx][y+dy] == Empty && state[x-dx][y-dy] == Empty)
        ||(state[x+4*dx][y+4*dy] == Empty && state[x+3*dx][y+3*dy] == player && state[x+2*dx][y+2*dy] == Empty && state[x+dx][y+dy] == player && state[x-dx][y-dy] == Empty))
        {
            jumpThreeFlag=true;
        }
    }
    if (x+dx >=0 && x+dx<BOARD_SIZE && x-4*dx >=0 && x-4*dx<BOARD_SIZE && y+dy>=0 && y+dy<BOARD_SIZE && y-4*dy>=0 && y-4*dy<BOARD_SIZE){
        if((state[x-4*dx][y-4*dy] == Empty && state[x-3*dx][y-3*dy] == player && state[x-2*dx][y-2*dy] == player && state[x-dx][y-dy] == Empty && state[x+dx][y+dy] == Empty)
            ||(state[x-4*dx][y-4*dy] == Empty && state[x-3*dx][y-3*dy] == player && state[x-2*dx][y-2*dy] == Empty && state[x-dx][y-dy] == player && state[x+dx][y+dy] == Empty))
        {
            jumpThreeFlag=true;
        }
    }
    if (x+3*dx >=0 && x+3*dx<BOARD_SIZE && x-2*dx >=0 && x-2*dx<BOARD_SIZE && y+3*dy>=0 && y+3*dy<BOARD_SIZE && y-2*dy>=0 && y-2*dy<BOARD_SIZE){
        if(state[x+3*dx][y+3*dy] == Empty && state[x+2*dx][y+2*dy] == player && state[x+dx][y+dy] == Empty && state[x-dx][y-dy] == player && state[x-2*dx][y-2*dy] == Empty)
        {
            jumpThreeFlag=true;
        }
    }
    if (x+2*dx >=0 && x+2*dx<BOARD_SIZE && x-3*dx >=0 && x-3*dx<BOARD_SIZE && y+2*dy>=0 && y+2*dy<BOARD_SIZE && y-3*dy>=0 && y-3*dy<BOARD_SIZE){
        if(state[x-3*dx][y-3*dy] == Empty && state[x-2*dx][y-2*dy] == player && state[x-dx][y-dy] == Empty && state[x+dx][y+dy] == player && state[x+2*dx][y+2*dy] == Empty)
        {
            jumpThreeFlag=true;
        }
    }
    if (jumpThreeFlag){
        return 3;
    }


    // 正向检查
    int i = x + dx;
    int j = y + dy;
    while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
    {
        if (state[i][j] == player)
        {
            consecutive++;
            i += dx;
            j += dy;
        }
        else
        {
            if (state[i][j] == Empty)
                openEnds++;
            break;
        }
    }

    // 反向检查
    i = x - dx;
    j = y - dy;
    while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
    {
        if (state[i][j] == player)
        {
            consecutive++;
            i -= dx;
            j -= dy;
        }
        else
        {
            if (state[i][j] == Empty)
                openEnds++;
            break;
        }
    }

    if (consecutive >= 5)
    {
        return 6; // 连五
    }

    if (consecutive == 4)
    {
        if (openEnds == 2)
            return 5; // 活四
        else if (openEnds == 1)
            return 4; // 冲四
    }

    if (consecutive == 3)
    {
        if (openEnds == 2)
            return 3; // 活三
        else if (openEnds == 1)
            return 2; // 眠三
    }

    if (consecutive == 2)
    {
        if (openEnds == 2)
            return 2; // 活二
        if (openEnds == 1)
            return 1;
    }

    // 其他情况返回 0，不计分
    return 0;
}

bool AIWorker::checkWin(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
    if (x == -1 || y == -1)
    {
        return false;
    }
    // 检查四个方向
    if (countConsecutive(state, x, y, 1, 0, player) >= 5) return true;    // 水平
    if (countConsecutive(state, x, y, 0, 1, player) >= 5) return true;    // 垂直
    if (countConsecutive(state, x, y, 1, 1, player) >= 5) return true;    // 正斜线
    if (countConsecutive(state, x, y, 1, -1, player) >= 5) return true;   // 反斜线
    return false;
}

bool AIWorker::checkThree(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
    if (x == -1 || y == -1)
    {
        return false;
    }
    // 检查四个方向
    if (countConsecutive(state, x, y, 1, 0, player) >= 3) return true;    // 水平
    if (countConsecutive(state, x, y, 0, 1, player) >= 3) return true;    // 垂直
    if (countConsecutive(state, x, y, 1, 1, player) >= 3) return true;    // 正斜线
    if (countConsecutive(state, x, y, 1, -1, player) >= 3) return true;   // 反斜线
    return false;
}

bool AIWorker::checkFour(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
    if (x == -1 || y == -1)
    {
        return false;
    }
    // 检查四个方向
    if (countConsecutive(state, x, y, 1, 0, player) >= 4) return true;    // 水平
    if (countConsecutive(state, x, y, 0, 1, player) >= 4) return true;    // 垂直
    if (countConsecutive(state, x, y, 1, 1, player) >= 4) return true;    // 正斜线
    if (countConsecutive(state, x, y, 1, -1, player) >= 4) return true;   // 反斜线
    return false;
}

int AIWorker::countConsecutive(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, int player)
{
    int count = 1; // 包含当前位置

    // 正向
    int i = x + dx;
    int j = y + dy;
    while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && state[i][j] == player)
    {
        count++;
        i += dx;
        j += dy;
    }

    // 反向
    i = x - dx;
    j = y - dy;
    while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && state[i][j] == player)
    {
        count++;
        i -= dx;
        j -= dy;
    }

    return count;
}


int AIWorker::minimax(int state[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, bool maximizingPlayer, int lastMoveX, int lastMoveY)
{
    int player = maximizingPlayer ? aiColor : ((aiColor == White) ? Black : White);
    int opponent = (player == White) ? Black : White;


    if (lastMoveX != -1 && lastMoveY != -1)
    {
        if (checkWin(state, lastMoveX, lastMoveY, opponent))
        {
            return maximizingPlayer ? (std::numeric_limits<int>::min() + depth)
                                    : (std::numeric_limits<int>::max() - depth);
        }
    }

    // 终止
    if (depth <= 0)
    {
        return evaluate(state, aiColor);
    }

    // 生成
    std::vector<std::pair<int, int>> moves = generateCandidateMoves(state,player,depth);

    if (moves.empty())
    {
        return evaluate(state, aiColor);
    }

    if (maximizingPlayer)
    {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : moves)
        {
            // 放置棋子
            state[move.first][move.second] = player;

            int eval = minimax(state, depth - 1, alpha, beta, false, move.first, move.second);

            // 撤销
            state[move.first][move.second] = Empty;

            if (eval > maxEval)
            {
                maxEval = eval;
            }
            if (maxEval > alpha)
            {
                alpha = maxEval;
            }
            if (beta <= alpha)
            {
                break; // Beta 剪枝
            }
        }
        return maxEval;
    }
    else
    {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : moves)
        {
            // 放置棋子
            state[move.first][move.second] = player;

            int eval = minimax(state, depth - 1, alpha, beta, true, move.first, move.second);

            // 撤销棋子
            state[move.first][move.second] = Empty;

            if (eval < minEval)
            {
                minEval = eval;
            }
            if (minEval < beta)
            {
                beta = minEval;
            }
            if (beta <= alpha)
            {
                break; // Alpha 剪枝
            }
        }
        return minEval;
    }
}

std::vector<std::pair<int, int>> AIWorker::generateCandidateMoves(int state[BOARD_SIZE][BOARD_SIZE], int player, int depthNow)
{
    std::vector<std::pair<int, int>> candidateMoves;
    std::vector<std::pair<int, int>> attackMoves;
    std::vector<std::pair<int, int>> defenseMoves;
    std::vector<std::pair<int, int>> otherMoves;
    std::vector<std::pair<int, std::pair<int, int>>> sortedOtherMoves;

    //有效的候选位置
    bool validMoves[BOARD_SIZE][BOARD_SIZE] = { false };

    // 找周边两格
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (state[i][j] != Empty)
            {
                // 标记周围两格内的空位
                for (int dx = -2; dx <= 2; ++dx)
                {
                    for (int dy = -2; dy <= 2; ++dy)
                    {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < BOARD_SIZE && nj >= 0 && nj < BOARD_SIZE)
                        {
                            if (state[ni][nj] == Empty)
                            {
                                validMoves[ni][nj] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    // 收集走法
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (validMoves[i][j])
            {
                candidateMoves.emplace_back(i, j);
            }
        }
    }

    // 如果没有棋子，则选择中心点
    if (candidateMoves.empty())
    {
        candidateMoves.emplace_back(BOARD_SIZE / 2, BOARD_SIZE / 2);
    }

    // 分类走法
    for (const auto& move : candidateMoves)
    {
        int x = move.first;
        int y = move.second;

        // 模拟放置AI的棋子
        state[x][y] = player;
        bool isAttack = checkAttackMove(state, x, y, player, depthNow);
        state[x][y] = Empty;

        // 模拟放置对手的棋子
        int opponent = (player == White) ? Black : White;
        state[x][y] = opponent;
        bool isDefense = checkAttackMove(state, x, y, opponent, depthNow);
        state[x][y] = Empty;

        if (isAttack)
        {
            attackMoves.emplace_back(x, y);
        }
        else if (isDefense)
        {
            defenseMoves.emplace_back(x, y);
        }
        else if (depthNow >= depthKillSearch - depthComplete)
        {
            otherMoves.emplace_back(x, y);
        }
    }

    // 按优先级排序：攻击 > 防守 > 其他
    std::vector<std::pair<int, int>> sortedMoves;
    sortedMoves.reserve(attackMoves.size() + defenseMoves.size() + otherMoves.size());

    sortedMoves.insert(sortedMoves.end(), attackMoves.begin(), attackMoves.end());
    sortedMoves.insert(sortedMoves.end(), defenseMoves.begin(), defenseMoves.end());

    // 靠近棋盘中心的otherMoves优先
    if (depthNow >= depthKillSearch - depthComplete){
        int centerX = BOARD_SIZE / 2;
        int centerY = BOARD_SIZE / 2;
        sortedOtherMoves.reserve(otherMoves.size());
        for (const auto& move : otherMoves){
            int x = move.first;
            int y = move.second;
            int distanceToCenter = std::abs(x - centerX) + std::abs(y - centerY);
            int score = -distanceToCenter; // 离中心越近，得分越高
            sortedOtherMoves.emplace_back(std::make_pair(score, move));
        }
        std::sort(sortedOtherMoves.begin(), sortedOtherMoves.end(), [](const auto &a, const auto &b) {
            return a.first > b.first;
        });
        otherMoves.clear();
        otherMoves.reserve(sortedOtherMoves.size());
        for (const auto &movePair : sortedOtherMoves){
            otherMoves.emplace_back(movePair.second);
        }
        sortedMoves.insert(sortedMoves.end(), otherMoves.begin(), otherMoves.end());

        // 限制候选走法的数量
        int maxCandidates = attackMoves.size() + defenseMoves.size() + 10;
        if (sortedMoves.size() > maxCandidates)
        {
            sortedMoves.resize(maxCandidates);
        }
    }
    return sortedMoves;
}

        /*
        // 对每个候选走法进行快速评估
        std::vector<std::pair<int, std::pair<int, int>>> scoredMoves;

        int player = aiColor;
        int centerX = BOARD_SIZE / 2;
        int centerY = BOARD_SIZE / 2;

        for (const auto& move : moves)
        {
            int x = move.first;
            int y = move.second;

            int score = 0;

            // 评估当前走法的得分
            state[x][y] = player;
            score = evaluate(state, aiColor);
            state[x][y] = Empty;

            // 往中间下余地更大也有利于剪枝，所以对越远位置适当减分
            int distanceToCenter = std::abs(x - centerX) + std::abs(y - centerY);
            score -= distanceToCenter * 50;

            scoredMoves.push_back({score, move});
        }
        // 按得分从高到低排序
        std::sort(scoredMoves.begin(), scoredMoves.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
        });

        int maxCandidates = 20; // 候选走法数量
        if (scoredMoves.size() > maxCandidates)
        {
            scoredMoves.resize(maxCandidates);
        }

        // 提取排序后的走法
        std::vector<std::pair<int, int>> sortedMoves;
        for (const auto& scoredMove : scoredMoves)
        {
            sortedMoves.push_back(scoredMove.second);
        }

        return sortedMoves;*/ //错的

    bool AIWorker::checkAttackMove(int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player ,int depthNow)
    {
        // 检查四个方向
        for (int dir = 0; dir < 4; ++dir)
        {
            int dx = 0, dy = 0;
            switch (dir)
            {
            case 0: dx = 1; dy = 0; break;  // 水平
            case 1: dx = 0; dy = 1; break;  // 垂直
            case 2: dx = 1; dy = 1; break;  // 正斜线
            case 3: dx = 1; dy = -1; break; // 反斜线
            }

            int pattern = evaluatePattern(state, x, y, dx, dy, player);
            if (pattern >= 3 && depthNow>depthKillSearch-depthComplete) // 活三及以上
            {
                return true;
            }
            if (pattern >=4 && depthNow<=depthKillSearch-depthComplete)
            {
                return true;
            }
        }
        return false;
    }


int AIWorker::countDoubleLiveThree(int state[BOARD_SIZE][BOARD_SIZE], int player)
{
    int liveThreeCount = 0;

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        for (int y = 0; y < BOARD_SIZE; ++y)
        {
            if (state[x][y] == Empty)
            {
                // 模拟放置棋子
                state[x][y] = player;
                bool isLiveThree = false;

                // 检查所有方向是否形成活三
                for (int dir = 0; dir < 4; ++dir)
                {
                    int dx = 0, dy = 0;
                    switch (dir)
                    {
                    case 0: dx = 1; dy = 0; break;
                    case 1: dx = 0; dy = 1; break;
                    case 2: dx = 1; dy = 1; break;
                    case 3: dx = 1; dy = -1; break;
                    }
                    if (evaluatePattern(state, x, y, dx, dy, player) == 3) // 活三
                    {
                        isLiveThree = true;
                        break;
                    }
                }

                if (isLiveThree)
                {
                    liveThreeCount++;
                }

                // 撤销模拟
                state[x][y] = Empty;
            }
        }
    }

    return liveThreeCount;
}

// 检测双冲四
int AIWorker::countDoubleBlockedFour(int state[BOARD_SIZE][BOARD_SIZE], int player)
{
    int blockedFourCount = 0;

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        for (int y = 0; y < BOARD_SIZE; ++y)
        {
            if (state[x][y] == Empty)
            {
                // 模拟放置棋子
                state[x][y] = player;
                bool isBlockedFour = false;

                // 检查所有方向是否形成冲四
                for (int dir = 0; dir < 4; ++dir)
                {
                    int dx = 0, dy = 0;
                    switch (dir)
                    {
                    case 0: dx = 1; dy = 0; break;
                    case 1: dx = 0; dy = 1; break;
                    case 2: dx = 1; dy = 1; break;
                    case 3: dx = 1; dy = -1; break;
                    }
                    if (evaluatePattern(state, x, y, dx, dy, player) == 4) // 眠四
                    {
                        isBlockedFour = true;
                        break;
                    }
                }

                if (isBlockedFour)
                {
                    blockedFourCount++;
                }

                // 撤销模拟
                state[x][y] = Empty;
            }
        }
    }

    return blockedFourCount;
}

// 检测冲四 + 活三
int AIWorker::countLiveThreePlusBlockedFour(int state[BOARD_SIZE][BOARD_SIZE], int player)
{
    int combinationCount = 0;

    for (int x1 = 0; x1 < BOARD_SIZE; ++x1)
    {
        for (int y1 = 0; y1 < BOARD_SIZE; ++y1)
        {
            if (state[x1][y1] == Empty)
            {
                // 模拟放置棋子
                state[x1][y1] = player;
                bool hasLiveThree = false;
                bool hasBlockedFour = false;

                // 检查所有方向是否形成活三或冲四
                for (int dir = 0; dir < 4; ++dir)
                {
                    int dx = 0, dy = 0;
                    switch (dir)
                    {
                    case 0: dx = 1; dy = 0; break;
                    case 1: dx = 0; dy = 1; break;
                    case 2: dx = 1; dy = 1; break;
                    case 3: dx = 1; dy = -1; break;
                    }
                    int pattern = evaluatePattern(state, x1, y1, dx, dy, player);
                    if (pattern == 3) // 活三
                        hasLiveThree = true;
                    if (pattern == 4) // 冲四
                        hasBlockedFour = true;
                }

                if (hasLiveThree && hasBlockedFour)
                {
                    combinationCount++;
                }

                // 撤销模拟
                state[x1][y1] = Empty;
            }
        }
    }

    return combinationCount;
}


    QPoint AIWorker::normalMove()
{
    std::vector<std::pair<int, int>> moves = generateCandidateMoves(board, aiColor, depthKillSearch);
    if (moves.empty()) {
        return QPoint(-1, -1);
    }

    struct ThreadLocalData {
        int bestScore;
        std::vector<QPoint> bestMoves;
        ThreadLocalData() : bestScore(std::numeric_limits<int>::min()) {}
    };

    // 获取线程数量
    int threadCount = std::thread::hardware_concurrency();
    threadCount = threadCount > 0 ? threadCount : 8;
    std::vector<ThreadLocalData> threadData(threadCount);

    // 分配走法到各个线程
    std::vector<std::vector<std::pair<int, int>>> threadMoves(threadCount);
    for (size_t i = 0; i < moves.size(); ++i) {
        threadMoves[i % threadCount].emplace_back(moves[i]);
    }

    // 创建并行任务
    std::vector<std::future<void>> futures;
    for (int t = 0; t < threadCount; ++t) {
        futures.emplace_back(std::async(std::launch::async, [&, t]() {
            for (const auto& move : threadMoves[t]) {
                int x = move.first;
                int y = move.second;
                int boardCopy[BOARD_SIZE][BOARD_SIZE];
                copyBoard(board, boardCopy);

                boardCopy[x][y] = aiColor;

                int score = minimax(boardCopy, depthKillSearch - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, x, y);

                // 更新线程局部的最佳得分和走法
                if (score > threadData[t].bestScore) {
                    threadData[t].bestScore = score;
                    threadData[t].bestMoves.clear();
                    threadData[t].bestMoves.emplace_back(x, y);
                }
                else if (score == threadData[t].bestScore) {
                    threadData[t].bestMoves.emplace_back(x, y);
                }
            }
        }));
    }

    // 等待所有线程完成
    for (auto& fut : futures) {
        fut.get();
    }

    // 合并所有线程的最佳得分和走法
    int globalBestScore = std::numeric_limits<int>::min();
    std::vector<QPoint> globalBestMoves;

    for (int t = 0; t < threadCount; ++t) {
        if (threadData[t].bestScore > globalBestScore) {
            globalBestScore = threadData[t].bestScore;
            globalBestMoves = threadData[t].bestMoves;
        }
        else if (threadData[t].bestScore == globalBestScore) {
            globalBestMoves.insert(globalBestMoves.end(), threadData[t].bestMoves.begin(), threadData[t].bestMoves.end());
        }
    }

    // 挣扎一下
    if (globalBestScore < -FIVE) {
        std::vector<std::pair<int, int>> defenseMoves;
        int opponent = (aiColor == White) ? Black : White;

        //查找对手的潜在威胁
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == Empty) {
                    board[i][j] = opponent;
                    if (checkWin(board, i, j, opponent)) {
                        defenseMoves.emplace_back(i, j);
                    }
                    board[i][j] = Empty;
                }
            }
        }

        if (!defenseMoves.empty()) {
            // 去重
            std::sort(defenseMoves.begin(), defenseMoves.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                if (a.first != b.first) return a.first < b.first;
                return a.second < b.second;
            });
            defenseMoves.erase(std::unique(defenseMoves.begin(), defenseMoves.end()), defenseMoves.end());

            // 随机选择一个防守走法
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, static_cast<int>(defenseMoves.size()) - 1);
            auto selectedMove = defenseMoves[dis(gen)];
            return QPoint(selectedMove.first, selectedMove.second);
        }
    }

    if (!globalBestMoves.empty()) {
        // 去重
        std::sort(globalBestMoves.begin(), globalBestMoves.end(), [](const QPoint& a, const QPoint& b) {
            if (a.x() != b.x()) return a.x() < b.x();
            return a.y() < b.y();
        });
        globalBestMoves.erase(std::unique(globalBestMoves.begin(), globalBestMoves.end(), [](const QPoint& a, const QPoint& b) {
                                  return a.x() == b.x() && a.y() == b.y();
                              }), globalBestMoves.end());

        // 随机选择一个最佳走法
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(globalBestMoves.size()) - 1);
        return globalBestMoves[dis(gen)];
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(moves.size()) - 1);
    auto randMove = moves[dis(gen)];
    return QPoint(randMove.first, randMove.second);
}


QPoint AIWorker::interativeDeepeningMove()
{
    // 最小深度和最大深度
    const int minDepth = 1;
    const int maxDepth = 8;
    QPoint forcedWinMove(-1, -1);
    int minimalDepthToWin = maxDepth + 1; // 初始化为比最大深度大的值

    // 遍历每一层深度
    for (int depth = minDepth; depth <= maxDepth; ++depth)
    {
        // 生成候选走法
        std::vector<std::pair<int, int>> moves = generateCandidateMoves(board, aiColor, depth);

        if (moves.empty())
            continue;

        // 存储每个走法的结果
        struct MoveResult
        {
            QPoint move;
            bool isForcedWin;
        };

        std::vector<MoveResult> results;
        std::mutex resultsMutex;

        std::vector<std::future<void>> futures;
        for (const auto& move : moves)
        {
            futures.emplace_back(std::async(std::launch::async, [&, move]() {
                int x = move.first;
                int y = move.second;
                int boardCopy[BOARD_SIZE][BOARD_SIZE];
                copyBoard(board, boardCopy);

                boardCopy[x][y] = aiColor;
                int score = minimax(boardCopy, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, x, y);

                // 判断是否必胜
                bool isForcedWin = (score >= FIVE);

                // 保护性地将结果存储到共享资源中
                {
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results.push_back({QPoint(x, y), isForcedWin});
                }
            }));
        }

        // 等待所有线程完成
        for (auto& fut : futures)
            fut.get();

        // 检查是否有必胜走法
        for (const auto& res : results)
        {
            if (res.isForcedWin)
            {
                if (depth < minimalDepthToWin)
                {
                    forcedWinMove = res.move;
                    minimalDepthToWin = depth;
                }
            }
        }

        // 如果在当前深度找到了必胜走法就立即返回
        if (forcedWinMove.x() != -1 && forcedWinMove.y() != -1)
        {
            return forcedWinMove;
        }
    }
    return normalMove();
}
