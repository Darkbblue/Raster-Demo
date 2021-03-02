#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ssw(nullptr)
{
    // 窗体设置
    ui->setupUi(this);
    this->setFixedSize(1900, 950);

    // 内容初始化
    Clear();

    // 连接信号/槽
    connect(&ssw, &ShapeSetWin::SetShape, this, &MainWindow::SetShape);

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
    SetButtonAccess();
}

void MainWindow::ClearBoardOnly() // 仅清空绘制信息，不清除原始图形
{
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
            board[x][y] = mark[x][y] = 0;
}

void MainWindow::SetButtonAccess() // 设置按钮的活跃状态
{
    ui->pbLine1->setEnabled(false);
    ui->pbLine2->setEnabled(false);
    ui->pbLine3->setEnabled(false);
    ui->pbCircle->setEnabled(false);
    ui->pbPoly1->setEnabled(false);
    ui->pbPoly2->setEnabled(false);
    ui->pbPoly3->setEnabled(false);
    ui->pbPoly4->setEnabled(false);

    if (shape.active)
    {
        if (shape.type == 0) // 直线段
        {
            ui->pbLine1->setEnabled(true);
            ui->pbLine2->setEnabled(true);
            ui->pbLine3->setEnabled(true);
        }
        else if (shape.type == 1) // 圆形
            ui->pbCircle->setEnabled(true);
        else if (shape.type == 2) // 多边形
        {
            ui->pbPoly1->setEnabled(true);
            ui->pbPoly2->setEnabled(true);
            ui->pbPoly3->setEnabled(true);
            ui->pbPoly4->setEnabled(true);
        }
    }
}

// 核心算法
void MainWindow::Line1(Point ps, Point pt) // 直线段 数值微分法
{
    float k = (pt.y - ps.y) / float(pt.x - ps.x); // 斜率
    float y = ps.y; // 增量式计算的 y 坐标
    for (int x = ps.x; x <= pt.x; x++) // 沿着 x 轴自左向右扫描
    {
        board[x][int(y + 0.5)] = 1;
        y += k;
        update();
    }
}

void MainWindow::SetShape(int type, int p1x, int p1y, int p2x, int p2y, // 设置图形
                  int p3x, int p3y, int p4x, int p4y)
{
    if (type == 0)
        shape.SetLine(Point(p1x, p1y), Point(p2x, p2y));
    else if (type == 1)
        shape.SetCircle(Point(p1x, p1y), p2x);
    else if (type == 2)
        shape.SetPolygon(Point(p1x, p1y), Point(p2x, p2y),
                          Point(p3x, p3y), Point(p4x, p4y));
    update();
    SetButtonAccess();
}

void MainWindow::on_pbSetShape_clicked()
{
    ssw.show();
}

void MainWindow::on_pbLine1_clicked()
{
    ClearBoardOnly();
    Line1(shape.p1, shape.p2);
}
