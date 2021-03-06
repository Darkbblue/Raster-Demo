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

    // 绘制辅助信息
    p.setPen(Qt::gray);
    p.setBrush(Qt::gray);
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
        {
            if (mark[x][y] == 1)
                p.drawRect(shape.TransformX(x) - 5, LATTICE / 2 + shape.TransformY(y) - 5, 10, 10);
            if (markY[x][y] == 1)
                p.drawRect(LATTICE / 2 + shape.TransformX(x) - 5, shape.TransformY(y) - 5, 10, 10);
        }

    // 设置高亮像素
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
        {
            if (board[x][y] == 1) // 普通高亮
            {
                p.setPen(Qt::black);
                p.setBrush(Qt::black);
                p.drawRect(shape.TransformX(x) - 10, shape.TransformY(y) - 10, 20, 20);
            }
            else if (board[x][y] == 2) // frontier
            {
                p.setPen(Qt::red);
                p.setBrush(Qt::red);
                p.drawRect(shape.TransformX(x) - 10, shape.TransformY(y) - 10, 20, 20);
            }
        }

    // 绘制原始图形
    p.setPen(Qt::black);
    p.setBrush(Qt::transparent);
    if (shape.active) // 只有存在活跃图形时进行绘制
    {
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
            p.drawEllipse(QPoint(shape.TransformX(shape.ps.x), // 填充种子
                                 shape.TransformY(shape.ps.y)),
                          2, 2);
        }
    }
}

void MainWindow::Clear() // 清空图形数据
{
    shape.Clear();
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
            board[x][y] = mark[x][y] = markY[x][y] = 0;
    SetButtonAccess();
}

void MainWindow::ClearBoardOnly() // 仅清空绘制信息，不清除原始图形
{
    for (int x = 0; x < X_LIMIT; x++)
        for (int y = 0; y < Y_LIMIT; y++)
            board[x][y] = mark[x][y] = markY[x][y] = 0;
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

void MainWindow::Sleep(int time) // 延迟
{
    QEventLoop ev;
    QTimer::singleShot(time, &ev, SLOT(quit()));
    ev.exec();
}

void MainWindow::SwapPoint(Point & a, Point & b) // 交换两个点
{
    Point tmp = a;
    a = b;
    b = tmp;
}

void MainWindow::AutoUpdate(Point & buffer, Point next) // 自动维护 frontier 更新逻辑
{
    if (buffer.x != -1) // 将上一轮的 frontier 设置为普通着色像素
        board[buffer.x][buffer.y] =1;
    if (next.x != -1) // 更新 frontier 并设置为特殊着色像素
    {
        buffer = next; // 更新 frontier
        board[buffer.x][buffer.y] = 2; // 设置 frontier 为特殊着色像素
    }
    update(); // 触发重绘
    Sleep(500); // 等待
}

// 核心算法
void MainWindow::Line1(Point ps, Point pt) // 直线段 数值微分法
{
    int dyAbs = pt.y - ps.y > 0 ? pt.y - ps.y : ps.y - pt.y;
    int dxAbs = pt.x - ps.x > 0 ? pt.x - ps.x : ps.x - pt.x;
    if (dyAbs <= dxAbs) // 常规的更缓的线段，以 x 为基准扫描
    {
        if (ps.x > pt.x)
            SwapPoint(ps, pt);
        float k = (pt.y - ps.y) / float(pt.x - ps.x); // 斜率，线段的平缓保证了不会为无穷大
        float y = ps.y; // 增量式计算的 y 坐标
        Point buffer(-1, -1);
        for (int x = ps.x; x <= pt.x; x++) // 沿着 x 轴自左向右扫描
        {
            AutoUpdate(buffer, Point(x, int(y + 0.5)));
            y += k; // 计算后续走向
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
    else // 更陡峭的线段，以 y 为基准扫描
    {
        if (ps.y > pt.y)
            SwapPoint(ps, pt);
        float k = (pt.x - ps.x) / float(pt.y - ps.y); // 等效的斜率
        float x = ps.x;
        Point buffer(-1, -1);
        for (int y = ps.y; y <= pt.y; y++)
        {
            AutoUpdate(buffer, Point(int(x + 0.5), y));
            x += k;
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
}

void MainWindow::Line2(Point ps, Point pt) // 直线段 中点画线法
{
    int dyAbs = pt.y - ps.y > 0 ? pt.y - ps.y : ps.y - pt.y;
    int dxAbs = pt.x - ps.x > 0 ? pt.x - ps.x : ps.x - pt.x;
    if (dyAbs <= dxAbs) // 常规的更缓的线段，以 x 为基准扫描
    {
        if (ps.x > pt.x) // 确保从左到右绘制
            SwapPoint(ps, pt);
        int a = ps.y - pt.y; // 直线参数
        int b = pt.x - ps.x;
        int yInc = pt.y - ps.y > 0 ? 1 : -1; // 直线段在 y 向上是增大的吗
        int d = 2 * a + yInc * b; // 判据函数
        int inc1 = 2 * a; // 增量 1
        int inc2 = 2 * (a + yInc * b); // 增量 2
        int x = ps.x; // 初始位置
        int y = ps.y;

        Point buffer(-1, -1);
        mark[x + 1][yInc > 0 ? y : y - 1] = 1;
        AutoUpdate(buffer, Point(x, y));
        while (x < pt.x) // 执行线性扫描
        {
            if (0x80000000 & (yInc ^ d)) // 若中点在直线下方，取斜对角
            {
                x++;
                y += yInc;
                d += inc2;
            }
            else // 若中点在直线上方，取正右侧
            {
                x++;
                d += inc1;
            }
            mark[x + 1][yInc > 0 ? y : y - 1] = 1;
            AutoUpdate(buffer, Point(x, y));
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
    else // 以 y 为基准扫描
    {
        if (ps.y > pt.y) // 确保从左到右绘制
            SwapPoint(ps, pt);
        int a = ps.x - pt.x; // 直线参数
        int b = pt.y - ps.y;
        int xInc = pt.x - ps.x > 0 ? 1 : -1; // 直线段在 x 向上是增大的吗
        int d = 2 * a + xInc * b; // 判据函数
        int inc1 = 2 * a; // 增量 1
        int inc2 = 2 * (a + xInc * b); // 增量 2
        int x = ps.x; // 初始位置
        int y = ps.y;

        Point buffer(-1, -1);
        markY[xInc > 0 ? x : x - 1][y + 1] = 1;
        AutoUpdate(buffer, Point(x, y));
        while (y < pt.y) // 执行线性扫描
        {
            if (0x80000000 & (xInc ^ d)) // 若中点在直线下方，取斜对角
            {
                y++;
                x += xInc;
                d += inc2;
            }
            else // 若中点在直线上方，取正右侧
            {
                y++;
                d += inc1;
            }
            markY[xInc > 0 ? x : x - 1][y + 1] = 1;
            AutoUpdate(buffer, Point(x, y));
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
}

void MainWindow::Line3(Point ps, Point pt) // 直线段 Bresenham
{
    int dyAbs = pt.y - ps.y > 0 ? pt.y - ps.y : ps.y - pt.y;
    int dxAbs = pt.x - ps.x > 0 ? pt.x - ps.x : ps.x - pt.x;
    if (dyAbs <= dxAbs) // 常规的更缓的线段，以 x 为基准扫描
    {
        if (ps.x > pt.x) // 确保正向扫描
            SwapPoint(ps, pt);
        int yInc = pt.y - ps.y > 0 ? 1 : -1; // 直线在 y 向上是递增的吗
        int e = yInc * (ps.x - pt.x); // 判据 误差函数
        int inc1 = 2 * (pt.y - ps.y); // 增量 1，适用于取正右侧点
        int inc2 = 2 * e; // 增量 2，适用于取右上方点时进行额外计算
        int x = ps.x; // 扫描起点
        int y = ps.y;
        Point buffer(-1, -1);
        mark[x + 1][yInc > 0 ? y : y - 1] = 1;
        AutoUpdate(buffer, Point(x, y));
        while (x < pt.x)
        {
            x++;
            e += inc1;
            if ((e ^ yInc) >= 0)
            {
                y += yInc;
                e += inc2;
            }
            mark[x + 1][yInc > 0 ? y : y - 1] = 1;
            AutoUpdate(buffer, Point(x, y));
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
    else // 以 y 为基准扫描
    {
        if (ps.y > pt.y) // 确保正向扫描
            SwapPoint(ps, pt);
        int xInc = pt.x - ps.x > 0 ? 1 : -1; // 直线在 x 向上是递增的吗
        int e = xInc * (ps.y - pt.y); // 判据 误差函数
        int inc1 = 2 * (pt.x - ps.x); // 增量 1，适用于取正右侧点
        int inc2 = 2 * e; // 增量 2，适用于取右上方点时进行额外计算
        int x = ps.x; // 扫描起点
        int y = ps.y;
        Point buffer(-1, -1);
        markY[xInc > 0 ? x : x - 1][y + 1] = 1;
        AutoUpdate(buffer, Point(x, y));
        while (y < pt.y)
        {
            y++;
            e += inc1;
            if ((e ^ xInc) >= 0)
            {
                x += xInc;
                e += inc2;
            }
            markY[xInc > 0 ? x : x - 1][y + 1] = 1;
            AutoUpdate(buffer, Point(x, y));
        }
        AutoUpdate(buffer, Point(-1, -1));
    }
}

void MainWindow::Circle(Point pc, int r) // 圆 中点画圆法
{
    // 在坐标原点处计算，绘制时再平移到实际位置
    int x = 0; // 从圆的最顶部开始，向右扫描八分之一圆
    int y = r;
    int e = 10 - (r << 3); // 判据函数
    auto moveToPc = [&pc](Point origin) // 将原点处的计算结果平移到实际位置
    {
        return Point(origin.x + pc.x, origin.y + pc.y);
    };
    auto drawSymetrics = [&](Point origin) // 绘制对称位置的点
    {
        for (int i = 0; i < 4; i++)
        {
            Point target = Point(origin.x * (-1 + (i & 0x2)), origin.y * (-1 + ((i << 1) & 0x2)));
            Point p1 = moveToPc(target);
            Point p2 = moveToPc(Point(target.y, target.x));
            board[p1.x][p1.y] = board[p2.x][p2.y] = 1;
        }
    };
    Point buffer(-1, -1);
    drawSymetrics(Point(x, y));
    mark[moveToPc(Point(x, y)).x + 1][moveToPc(Point(x, y)).y - 1] = 1;
    AutoUpdate(buffer, moveToPc(Point(x, y)));
    while (x < y) // 仅扫描八分之一圆周
    {
        if (e < 0) // 取正右侧点
            e += 24 + (x << 4);
        else // 取对角线方向的点
        {
            e += 40 + ((x - y) << 4);
            y--;
        }
        x++;
        drawSymetrics(Point(x, y));
        mark[moveToPc(Point(x, y)).x + 1][moveToPc(Point(x, y)).y - 1] = 1;
        AutoUpdate(buffer, moveToPc(Point(x, y)));
    }
    AutoUpdate(buffer, Point(-1, -1));
}

void MainWindow::PolyFill1() // 区域填充 递归算法
{
    Point buffer(-1, -1);
    PolyFill1Sub(shape.ps.x, shape.ps.y, buffer);
    AutoUpdate(buffer, Point(-1, -1));
}

void MainWindow::PolyFill1Sub(int x, int y, Point & buffer) // 子函数
{
    if (board[x][y] == 0) // 若尚未填色
    {
        AutoUpdate(buffer, Point(x, y));
        PolyFill1Sub(x, y + 1, buffer);
        PolyFill1Sub(x, y - 1, buffer);
        PolyFill1Sub(x - 1, y, buffer);
        PolyFill1Sub(x + 1, y, buffer);
    }
}

void MainWindow::PolyFill2() // 区域填充 扫描线算法
{
    std::vector<Point> stack; // 记录待完成扫描线的栈
    stack.push_back(shape.ps); // 种子入栈
    Point buffer(-1, -1);
    while (!stack.empty()) // 在栈空前循环
    {
        Point ps = stack.back(); // 取出栈顶
        stack.pop_back();
        int x = ps.x;
        int y = ps.y;
        int xr; // 当前扫描线上到达的最右侧位置
        int xl; // 当前扫描线上到达的最左侧位置
        // 填充当前扫描线
        while (board[x][y] == 0) // 向右填充
        {
            AutoUpdate(buffer, Point(x, y));
            x++;
        }
        xr = x - 1;
        x = ps.x - 1;
        while (board[x][y] == 0) // 向左填充
        {
            AutoUpdate(buffer, Point(x, y));
            x--;
        }
        xl = x + 1;
        // 向相邻扫描线扩展
        y++; // 移动到上方相邻行
        for (int i = 0; i < 2; i++, y -= 2) // 先处理上方，再处理下方
        {
            x = xl; // 回到最左侧
            while (x <= xr) // 在到达最右侧之前
            {
                bool toSpan = false; // 是否需要扩展
                while (board[x][y] == 0) // 扫描出需要填充的区间
                {
                    toSpan = true;
                    x++;
                }
                if (toSpan) // 如果真的进入过上面的循环
                    stack.push_back(Point(x - 1, y)); // 得到一个新的扫描线种子，入栈
                while (board[x][y] != 0 && x <= xr) // 快速跳过不需要填充的区间
                    x++;
            }
        }
    }
    AutoUpdate(buffer, Point(-1, -1));
}

void MainWindow::SetShape(int type, int p1x, int p1y, int p2x, int p2y, // 设置图形
                  int p3x, int p3y, int p4x, int p4y, int p5x, int p5y)
{
    Clear();
    if (type == 0)
        shape.SetLine(Point(p1x, p1y), Point(p2x, p2y));
    else if (type == 1)
        shape.SetCircle(Point(p1x, p1y), p2x);
    else if (type == 2)
        shape.SetPolygon(Point(p1x, p1y), Point(p2x, p2y),
                          Point(p3x, p3y), Point(p4x, p4y));
    else if (type == 3)
    {
        shape.SetSeed(Point(p5x, p5y));
        SetShape(2, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, p5x, p5y);
    }
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

void MainWindow::on_pbLine2_clicked()
{
    ClearBoardOnly();
    Line2(shape.p1, shape.p2);
}

void MainWindow::on_pbLine3_clicked()
{
    ClearBoardOnly();
    Line3(shape.p1, shape.p2);
}

void MainWindow::on_pbCircle_clicked()
{
    ClearBoardOnly();
    Circle(shape.p1, shape.p2.x);
}

void MainWindow::on_pbPoly3_clicked()
{
    ClearBoardOnly();
    Line1(shape.p1, shape.p2);
    Line1(shape.p2, shape.p3);
    Line1(shape.p3, shape.p4);
    Line1(shape.p1, shape.p4);
    PolyFill1();
}

void MainWindow::on_pbPoly4_clicked()
{
    ClearBoardOnly();
    Line1(shape.p1, shape.p2);
    Line1(shape.p2, shape.p3);
    Line1(shape.p3, shape.p4);
    Line1(shape.p1, shape.p4);
    PolyFill2();
}
