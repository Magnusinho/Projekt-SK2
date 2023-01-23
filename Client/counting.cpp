#include "counting.h"
#include "ui_counting.h"

void counting::setValue(int value)
{
    number = value;
    ui->lcdNumber->display(number);
}

counting::counting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::counting)
{
    ui->setupUi(this);
}

counting::~counting()
{
    delete ui;
}
