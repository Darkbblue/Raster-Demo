#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 窗体设置
    ui->setupUi(this);
    this->setFixedSize(1900, 950);

    // 内容初始化
    Clear();

    // 测试
/*    shape.SetCircle(Point(10, 7), 7);
    board[1][1] = 1;
    mark[2][2] = 1;*/

    // 其他
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    // 初始化
    QPainter p(this);

    // 刷新画布
    {
        p.setPen(QColor(240, 240, 240, 255)); // 均设置为白色
        p.setBrush(QColor(240, 240, 240, 255));
        p.drawRect(0, 0, width(), width()); // 覆盖全屏
    }

    // 绘制像素网格
    p.setPen(Qt::gray);
    p.setBrush(QColor(240, 240, 240, 255));
    for (int x = LEFT_MARGIN; x < width() - LATTICE; x += LATTICE)
        for (int y = UPPER_MARGIN; y < height() - LATTICE; y += LATTICE)
            p.drawRect(x, y, LATTICE, LATTICE);

    // 设置高亮像素
    p.setPen(Qt::gray);
    p.setBrush(Qt::gray);
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
            if (mark[x][y] == 1)
                p.drawRect(shape.TransformX(x) - 5, LATTICE / 2 + shape.TransformY(y) - 5, 10, 10);

    // 绘制辅助信息
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
        {
            if (board[x][y] == 1) // 普通高亮
            {
                p.setPen(Qt::black);
                p.setBrush(Qt::black);
                p.drawRect(shape.TransformX(x) - 10, shape.TransformY(y) - 10, 20, 20);
            }
        }

    // 绘制原始图形
    p.setPen(Qt::black);
    p.setBrush(Qt::transparent);
    if (shape.active) // 只有存在活跃图形时进行绘制
    {
        qDebug() << "*";
        if (shape.type == 0) // 直线段
            p.drawLine(shape.TransformX(shape.p1.x),
                       shape.TransformY(shape.p1.y),
                       shape.TransformX(shape.p2.x),
                       shape.TransformY(shape.p2.y));
        else if (shape.type == 1) // 圆形
        {
            p.drawEllipse(QPoint(shape.TransformX(shape.p1.x),
                                 shape.TransformY(shape.p1.y)),
                          shape.TransformR(shape.p2.x),
                          shape.TransformR(shape.p2.x));
            p.drawEllipse(QPoint(shape.TransformX(shape.p1.x), // 标注圆心位置
                                 shape.TransformY(shape.p1.y)),
                          2, 2);
        }
        else if (shape.type == 2) // 多边形
        {
            p.drawLine(shape.TransformX(shape.p1.x),
                       shape.TransformY(shape.p1.y),
                       shape.TransformX(shape.p2.x),
                       shape.TransformY(shape.p2.y));
            p.drawLine(shape.TransformX(shape.p2.x),
                       shape.TransformY(shape.p2.y),
                       shape.TransformX(shape.p3.x),
                       shape.TransformY(shape.p3.y));
            p.drawLine(shape.TransformX(shape.p3.x),
                       shape.TransformY(shape.p3.y),
                       shape.TransformX(shape.p4.x),
                       shape.TransformY(shape.p4.y));
            p.drawLine(shape.TransformX(shape.p1.x),
                       shape.TransformY(shape.p1.y),
                       shape.TransformX(shape.p4.x),
                       shape.TransformY(shape.p4.y));
        }
    }
}

void MainWindow::Clear() // 清空图形数据
{
    shape.Clear();
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
            board[x][y] = mark[x][y] = 0;
}
