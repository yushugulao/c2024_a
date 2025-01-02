
//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class BoardWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonUndo_clicked();      // 与“悔棋”按钮对接
    void on_buttonReset_clicked();     // 与“重新开始”按钮对接


    void on_buttonUserFirst_clicked();
    void on_buttonAIFirst_clicked();

    // AI 线程相关
    void onAIStarted();
    void onAIFinished();

private:
    Ui::MainWindow *ui;
    BoardWidget *board;
};

#endif // MAINWINDOW_H
