#include "GraphDialog2.h"
#include "ui_GraphDialog2.h"

GraphDialog2::GraphDialog2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDialog2)
{
    ui->setupUi(this);
}

GraphDialog2::~GraphDialog2()
{
    delete ui;
}
