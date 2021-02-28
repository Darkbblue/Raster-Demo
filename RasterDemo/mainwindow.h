#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include <QDebug>

#include "generalstruct.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);

protected:
    void Clear(); // 清空图形数据

private:
    Ui::MainWindow *ui;

    // 记录所有需要绘制的元素
    Shape shape; // 原始图形
    int board[X_LIMIT][Y_LIMIT]; // 大像素的颜色记录
    int mark[X_LIMIT][Y_LIMIT]; // 特殊标记，例如中点标记，绘制在上述位置下方一半像素宽度处
};

#endif // MAINWINDOW_H
