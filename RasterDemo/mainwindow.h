#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include <QDebug>

#include "generalstruct.h"
#include "shapesetwin.h"

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
    // 管理功能
    void Clear(); // 清空图形数据
    void ClearBoardOnly(); // 仅清空绘制信息，不清除原始图形
    void SetButtonAccess(); // 设置按钮的活跃状态
    void Sleep(int time = 1000); // 延迟
    void SwapPoint(Point & a, Point & b); // 交换两个点
    void AutoUpdate(Point & buffer, Point next); // 自动维护 frontier 更新逻辑

    // 核心算法
    void Line1(Point ps, Point pt); // 直线段 数值微分法
    void Line2(Point ps, Point pt); // 直线段 中点画线法
    void Line3(Point ps, Point pt); // 直线段 Bresenham
    void Circle(Point pc, int r); // 圆 中点画圆法
    void PolyFill1(); // 区域填充 递归算法
    void PolyFill1Sub(int x, int y, Point & buffer); // 子函数

protected slots:
    void SetShape(int type, int p1x, int p1y, int p2x, int p2y, // 设置图形
                  int p3x, int p3y, int p4x, int p4y, int p5x, int p5y);

private slots:
    void on_pbSetShape_clicked();

    void on_pbLine1_clicked();

    void on_pbLine2_clicked();

    void on_pbLine3_clicked();

    void on_pbCircle_clicked();

    void on_pbPoly3_clicked();

private:
    Ui::MainWindow *ui;

    // 子窗口
    ShapeSetWin ssw;

    // 记录所有需要绘制的元素
    Shape shape; // 原始图形
    int board[X_LIMIT][Y_LIMIT]; // 大像素的颜色记录
    int mark[X_LIMIT][Y_LIMIT]; // 特殊标记，例如中点标记，绘制在上述位置下方一半像素宽度处
    int markY[X_LIMIT][Y_LIMIT]; // 沿 y 向绘制时使用的特殊标记，绘制在 board 位置右侧一半像素宽度处
};

#endif // MAINWINDOW_H
