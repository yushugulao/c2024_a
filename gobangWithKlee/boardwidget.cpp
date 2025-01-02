#include "boardwidget.h"
#include "aiworker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <ctime>
#include <cstdlib>
#include <QSoundEffect>
#include <iostream>
#include <windows.h>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent), worker(nullptr), workerThread(nullptr)
{
    resetGame();

    // 随机数
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    //可莉背景
    backgroundPixmap.load("qrc:/klee.png");
    // “思考中”提示
    thinkingLabel = new QLabel(this);
    thinkingLabel->setText("思考中...");
    thinkingLabel->setStyleSheet("QLabel { background-color : lightgray; color : red; }");
    thinkingLabel->setAlignment(Qt::AlignCenter);
    thinkingLabel->setVisible(false);

    // 将 thinkingLabel 置于顶部中央
    thinkingLabel->setGeometry((width() - 100) / 2, 10, 100, 30);

    this->setMinimumSize(600, 600);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 读取落子音效文件
    moveSound_.setSource(QUrl("qrc:/move_sound.wav"));
    moveSound_.setVolume(0.5);

    // 设置背景音乐并播放
    backMu.setSource(QUrl("qrc:/backMu.wav"));
    backMu.setLoopCount(QSoundEffect::Infinite);  // 设置为无限循环
    backMu.setVolume(0.7);
    backMu.play();  // 播放背景音乐
}

BoardWidget::~BoardWidget()
{
    resetAIWorker();
}

// 设置玩家是否执黑
void BoardWidget::setUserIsBlack(bool isBlack)
{
    userIsBlack = isBlack;
}

void BoardWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    thinkingLabel->setGeometry((width() - 100) / 2, 10, 100, 30);
}

void BoardWidget::resetGame()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            board[i][j] = Empty;
        }
    }
    IsPainted=0;
    // 清空悔棋栈
    while(!movesStack.empty()) movesStack.pop();
    while(!pieceTypeStack.empty()) pieceTypeStack.pop();
    IsPainted=0;

    // 重置 AI 线程
    resetAIWorker();

    // 更新界面
    update();
    kleeSounds.setVolume(0.7);
    kleeSoundFiles << "qrc:/kleeSounds/a.wav"
                   << "qrc:/kleeSounds/f.wav"
                   << "qrc:/kleeSounds/g.wav"
                   << "qrc:/kleeSounds/h.wav"
        << "qrc:/kleeSounds/c.wav"
        << "qrc:/kleeSounds/d.wav"
        << "qrc:/kleeSounds/e.wav";

    //做了一个随机选取音频播放的效果，不会播放最近播过的
    int randomIndex = QRandomGenerator::global()->bounded(kleeSoundFiles.size());
    QUrl soundUrl(kleeSoundFiles[randomIndex]);
    kleeSounds.setSource(soundUrl);
    kleeSounds.play();
    if (!userIsBlack) {
        startAIMove();
    }
}

void BoardWidget::adjustImageContrast(QImage &image, double contrast)
{
    int width = image.width();
    int height = image.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor pixelColor = image.pixelColor(x, y);

            // 获取RGB分量
            int r = pixelColor.red();
            int g = pixelColor.green();
            int b = pixelColor.blue();

            // 计算亮度
            int avg = (r + g + b) / 3;

            // 根据对比度公式调整亮度
            r = qBound(0, int(128 + contrast * (r - 128)), 255);
            g = qBound(0, int(128 + contrast * (g - 128)), 255);
            b = qBound(0, int(128 + contrast * (b - 128)), 255);

            // 更新像素颜色
            image.setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    int w = width();
    int h = height();
    QImage backgroundImage(":/klee2.jpg");

    if (!backgroundImage.isNull()) {
        adjustImageContrast(backgroundImage, 0.15);
        for (int y = 0; y < backgroundImage.height(); ++y) {
            for (int x = 0; x < backgroundImage.width(); ++x) {
                QColor color = backgroundImage.pixelColor(x, y);
                color.setAlphaF(color.alphaF() * 0.5);  // 调整透明度
                backgroundImage.setPixelColor(x, y, color);
            }
        }
        // 绘制调整后的图像
        painter.drawImage(0, 0, backgroundImage.scaled(w, h, Qt::KeepAspectRatioByExpanding));
    } else {
        std::cout << "背景图片加载失败" << std::endl;
    }

    // 计算网格大小
    int gridSize = qMin(w, h) / (BOARD_SIZE + 1);
    int margin = gridSize;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white, 1));

    // 绘制棋盘网格
    for (int i = 0; i < BOARD_SIZE; ++i) {
        // 垂直线
        painter.drawLine(margin + gridSize * i, margin, margin + gridSize * i, margin + gridSize * (BOARD_SIZE - 1));
        // 水平线
        painter.drawLine(margin, margin + gridSize * i, margin + gridSize * (BOARD_SIZE - 1), margin + gridSize * i);
    }

    // 绘制棋子
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != Empty) {
                if (board[i][j] == Black) {
                    painter.setBrush(QColor(77, 30, 23));
                    painter.setPen(QColor(77, 30, 23));
                } else if (board[i][j] == White) {
                    painter.setBrush(QColor(246, 225, 198));
                    painter.setPen(QColor(246, 225, 198));
                }
                painter.drawEllipse(
                    QPoint(margin + gridSize * i, margin + gridSize * j),
                    gridSize / 2 - 2,
                    gridSize / 2 - 2
                    );
                IsPainted+=1;
            }
        }
    }

    // 如果AI的最后一手落子，不论是黑子还是白子，标记为可莉色系下稍微不同一点的颜色
    if (lastAIMove != QPoint(-1, -1) && IsPainted>=16) {
        if (board[lastAIMove.x()][lastAIMove.y()] == Black){
            painter.setBrush(QColor(134,39,24));
            painter.setPen(QColor(134,39,24));
        }
        else{
            painter.setBrush(QColor(216,159,138));
            painter.setPen(QColor(216,159,138));
        }
        painter.drawEllipse(
            QPoint(margin + gridSize * lastAIMove.x(), margin + gridSize * lastAIMove.y()),
            gridSize / 2 - 2,
            gridSize / 2 - 2
            );
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        int w = width();
        int h = height();
        int sqaSize = qMin(w, h) / (BOARD_SIZE + 1);

        // 获取点击的位置
        int x = (event->pos().x() - sqaSize / 2) / sqaSize;
        int y = (event->pos().y() - sqaSize / 2) / sqaSize;

        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            if (board[x][y] == Empty) {

                int playerPiece = userIsBlack ? Black : White;
                board[x][y] = playerPiece;
                update();

                movesStack.push(QPoint(x, y));
                pieceTypeStack.push(playerPiece);
                moveSound_.play();

                // 是否胜利
                if (checkWin(board, x, y, playerPiece)) {
                    QMessageBox::information(this, "游戏结束", "你赢了！");
                    resetGame();
                    return;
                }

                // 是否平局
                if (isFull(board)) {
                    QMessageBox::information(this, "游戏结束", "平局！");
                    resetGame();
                    return;
                }

                // AI 下棋
                startAIMove();
            }
        }
    }
}

void BoardWidget::startAIMove()
{
    // 显示“思考中”
    thinkingLabel->setVisible(true);
    // 锁定鼠标
    setDisabled(true);

    emit AIStarted();  // 通知外部

    // 如果之前有未结束的 AI 计算，重置
    resetAIWorker();

    worker = new AIWorker();
    worker->setBoard(board);

    int aiPiece = userIsBlack ? White : Black;
    worker->setAiColor(aiPiece);

    // 创建线程
    workerThread = new QThread();
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started,    worker, &AIWorker::process);
    connect(worker,      &AIWorker::resultReady,this,   &BoardWidget::onAIMoveFinished);
    connect(worker,      &AIWorker::resultReady,workerThread, &QThread::quit);
    connect(workerThread,&QThread::finished,    worker, &QObject::deleteLater);
    connect(workerThread,&QThread::finished,    workerThread, &QObject::deleteLater);

    // 启动线程
    workerThread->start();
}

void BoardWidget::onAIMoveFinished(int x, int y)
{
    thinkingLabel->setVisible(false);
    setDisabled(false);

    emit AIFinished();
    worker = nullptr;
    workerThread = nullptr;

    if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE) {
        lastAIMove = QPoint(x, y);
        // AI 用和玩家相反的颜色
        int aiPiece = userIsBlack ? White : Black;

        board[x][y] = aiPiece;
        update();

        movesStack.push(QPoint(x, y));
        pieceTypeStack.push(aiPiece);

        if (moveSound_.status() == QSoundEffect::Ready) {
            moveSound_.play();
        } else {
            qDebug() << "Sound effect not ready, status:" << moveSound_.status();
        }

        // 检查 AI 是否胜利
        if (checkWin(board, x, y, aiPiece)) {
            QMessageBox::information(this, "游戏结束", "AI 赢了！");
            resetGame();
            return;
        }

        // 满 平局
        if (isFull(board)) {
            QMessageBox::information(this, "游戏结束", "平局！");
            resetGame();
            return;
        }
    } else {
        // 无可落子
        QMessageBox::information(this, "游戏结束", "平局！");
        resetGame();
        return;
    }
}

void BoardWidget::undoMove()
{
    if (movesStack.empty() || pieceTypeStack.empty()) {
        return;
    }

    QPoint lastMove = movesStack.top();
    int lastPiece = pieceTypeStack.top();
    pieceTypeStack.pop();
    movesStack.pop();

    // 撤销玩家的棋子
    board[lastMove.x()][lastMove.y()] = Empty;
    IsPainted-=1;

    // 如果是玩家的落子，记录AI上一步的落子
    if (lastPiece == Black || lastPiece == White) {
        if (!movesStack.empty()) {
            QPoint aiLastMove = movesStack.top();
            lastAIMove = aiLastMove;
            IsPainted-=1;
        }
    }

    update();
}

bool BoardWidget::checkWin(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
    // 四个方向
    if (countConsecutive(state, x, y, 1, 0, player) >= 5) return true;
    if (countConsecutive(state, x, y, 0, 1, player) >= 5) return true;
    if (countConsecutive(state, x, y, 1, 1, player) >= 5) return true;
    if (countConsecutive(state, x, y, 1, -1, player) >= 5) return true;
    return false;
}

int BoardWidget::countConsecutive(const int state[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, int player)
{
    int count = 1;
    int i = x + dx;
    int j = y + dy;
    while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && state[i][j] == player)
    {
        count++;
        i += dx;
        j += dy;
    }

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

bool BoardWidget::isFull(const int state[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (state[i][j] == Empty)
                return false;
    return true;
}

void BoardWidget::copyBoard(const int source[BOARD_SIZE][BOARD_SIZE], int dest[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            dest[i][j] = source[i][j];
}

void BoardWidget::resetAIWorker()
{
    if (workerThread)
    {
        workerThread->quit();
        workerThread->wait();
        workerThread = nullptr;
        worker = nullptr;
    }
}
