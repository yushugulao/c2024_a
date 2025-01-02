
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boardwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("和可莉一起来下棋吧o(≧▽≦)o");
    // 棋盘控件
    board = new BoardWidget(this);

    QVBoxLayout *vbox = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (vbox) {
        QWidget *placeholder = vbox->itemAt(1)->widget();
        if (placeholder) {
            placeholder->hide();
            vbox->removeWidget(placeholder);
            placeholder->deleteLater();
        }
        // 将 board 加入布局
        vbox->addWidget(board);
    }

    // 悔棋
    connect(ui->buttonUndo, &QPushButton::clicked, this, &MainWindow::on_buttonUndo_clicked);
    // 重新开始
    connect(ui->buttonReset, &QPushButton::clicked, this, &MainWindow::on_buttonReset_clicked);

    connect(ui->buttonUserFirst, &QPushButton::clicked, this, &MainWindow::on_buttonUserFirst_clicked);
    connect(ui->buttonAIFirst,   &QPushButton::clicked, this, &MainWindow::on_buttonAIFirst_clicked);

    connect(board, &BoardWidget::AIStarted,  this, &MainWindow::onAIStarted);
    connect(board, &BoardWidget::AIFinished, this, &MainWindow::onAIFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonUndo_clicked()
{
    board->undoMove();
}

void MainWindow::on_buttonReset_clicked()
{
    board->resetGame();
}

void MainWindow::onAIStarted()
{
    // 禁用
    ui->buttonUndo->setEnabled(false);
    ui->buttonReset->setEnabled(false);
    ui->buttonUserFirst->setEnabled(false);
    ui->buttonAIFirst->setEnabled(false);
}

void MainWindow::onAIFinished()
{
    //恢复
    ui->buttonUndo->setEnabled(true);
    ui->buttonReset->setEnabled(true);
    ui->buttonUserFirst->setEnabled(true);
    ui->buttonAIFirst->setEnabled(true);
}

void MainWindow::on_buttonUserFirst_clicked()
{
    // 玩家执黑
    board->setUserIsBlack(true);
    board->resetGame();
}

void MainWindow::on_buttonAIFirst_clicked()
{
    // 玩家执白, AI 执黑
    board->setUserIsBlack(false);
    board->resetGame();
}
