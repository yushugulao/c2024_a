
//boardwidget.h
#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QThread>
#include <QPoint>
#include <stack>
#include <QSoundEffect>
#include <cstdint>
#include <functional>
#include <QPixmap>

enum PieceType {
    Empty = 0,
    Black=1,
    White=2
};


class AIWorker;
class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    ~BoardWidget();

    // 重置游戏
    void resetGame();

    // 悔棋函数
    void undoMove();

    void setUserIsBlack(bool isBlack);

    uint64_t getZobristHash() const { return zobristHash; } // 获取当前哈希值

signals:
    void AIStarted();   // 通知外部
    void AIFinished();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onAIMoveFinished(int x, int y);

private:
    int board[15][15];
    QLabel *thinkingLabel;

    void adjustImageContrast(QImage &image, double contrast);
    QPixmap backgroundPixmap;

    // AI计算线程
    AIWorker *worker;
    QThread *workerThread;

    bool userIsBlack = true;

    // 记录走子
    std::stack<QPoint> movesStack;
    std::stack<int> pieceTypeStack;
    QPoint lastAIMove;
    int IsPainted=0;

    //落子音效
    QSoundEffect moveSound_;
    QSoundEffect kleeSounds;
    QSoundEffect kleeLose;
    QSoundEffect backMu;
    QStringList kleeSoundFiles;
    QStringList kleeLoseFiles;

    void startAIMove();
    bool checkWin(const int state[15][15], int x, int y, int player);
    int countConsecutive(const int state[15][15], int x, int y, int dx, int dy, int player);
    bool isFull(const int state[15][15]);
    void copyBoard(const int source[15][15], int dest[15][15]);
    void resetAIWorker();

    // Zobrist 相关
    uint64_t zobristHash; // 当前棋盘状态的哈希值
    uint64_t zobristTable[15][15][3];    // 随机数表：0-空, 1-黑子, 2-白子
    uint64_t zobristPlayerSide[2];   // 用于区分黑方(0)落子或白方(1)落子

    void initializeZobristTable(); // 初始化随机数表
    void updateZobristHash(int x, int y, int pieceType); // 更新哈希值
    void undoZobristHash(int x, int y, int pieceType);   // 撤销哈希值
};

#endif // BOARDWIDGET_H
