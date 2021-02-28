#include "shapesetwin.h"
#include "ui_shapesetwin.h"

ShapeSetWin::ShapeSetWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShapeSetWin)
{
    ui->setupUi(this);
}

ShapeSetWin::~ShapeSetWin()
{
    delete ui;
}

void ShapeSetWin::SendShape(int type) // 信号的中转函数，用来减少重复代码
{
    emit SetShape(type,
            ui->leP1X->text().toInt(),
            ui->leP1Y->text().toInt(),
            ui->leP2X->text().toInt(),
            ui->leP2Y->text().toInt(),
            ui->leP3X->text().toInt(),
            ui->leP3Y->text().toInt(),
            ui->leP4X->text().toInt(),
            ui->leP4Y->text().toInt());
}

void ShapeSetWin::on_pbLine_clicked()
{
    SendShape(0);
}

void ShapeSetWin::on_pbCircle_clicked()
{
    SendShape(1);
}

void ShapeSetWin::on_pbPoly_clicked()
{
    SendShape(2);
}
