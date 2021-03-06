#ifndef SHAPESETWIN_H
#define SHAPESETWIN_H

#include <QWidget>

namespace Ui {
class ShapeSetWin;
}

class ShapeSetWin : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeSetWin(QWidget *parent = nullptr);
    ~ShapeSetWin();

protected:
    void SendShape(int type); // 信号的中转函数，用来减少重复代码

signals:
    void SetShape(int type, int p1x, int p1y, int p2x, int p2y,
                  int p3x, int p3y, int p4x, int p4y, int p5x, int p5y);

private slots:
    void on_pbLine_clicked();

    void on_pbCircle_clicked();

    void on_pbPoly_clicked();

    void on_pbSeed_clicked();

private:
    Ui::ShapeSetWin *ui;
};

#endif // SHAPESETWIN_H
