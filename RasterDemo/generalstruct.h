#ifndef GENERALSTRUCT_H
#define GENERALSTRUCT_H

#define LATTICE         50      // 像素网格的单位宽度
#define LEFT_MARGIN     20      // 左侧边界距离
#define UPPER_MARGIN    75      // 上方边界距离
#define X_LIMIT         38      // X 向像素数量
#define Y_LIMIT         18      // Y 向像素数量

struct Point // 点
{
    int x;
    int y;
    Point(int X = 0, int Y = 0) : x(X), y(Y) {}
};

struct Shape // 图形，包含线段、圆形、多边形
{
    bool active = false; // 当前是否处于活跃状态
    int type; // 0-线段，1-圆形，3-多边形
    Point p1, p2; // 线段所用的两个端点
    Point p3, p4; // 多边形使用的额外顶点
    void Clear() { active = false; } // 清空图形
    void SetLine(Point start, Point end) // 设置为线段
    {
        active = true;
        type = 0;
        p1 = start;
        p2 = end;
    }
    void SetCircle(Point center, int r) // 设置为圆形
    {
        active = true;
        type = 1;
        p1 = center;
        p2 = Point(r, 0);
    }
    void SetPolygon(Point a, Point b, Point c, Point d) // 设置为多边形
    {
        active = true;
        type = 2;
        p1 = a;
        p2 = b;
        p3 = c;
        p4 = d;
    }
    int TransformX(int x) { return x * LATTICE + LEFT_MARGIN; } // 将宽像素意义下的X坐标转换为实际坐标
    int TransformY(int y) { return y * LATTICE + UPPER_MARGIN; } // 将宽像素意义下的Y坐标转换为实际坐标
    int TransformR(int r) { return r * LATTICE; } // 将宽像素意义下的其他参数转换为实际数值
};

#endif // GENERALSTRUCT_H
