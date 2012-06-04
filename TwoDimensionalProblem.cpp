#include "TwoDimensionalProblem.h"
#include "ui_TwoDimensionalProblem.h"
#include <QDebug>

TwoDimensionalProblem::TwoDimensionalProblem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoDimensionalProblem)
{
    ui->setupUi(this);
}

TwoDimensionalProblem::~TwoDimensionalProblem()
{
    delete ui;
}

void TwoDimensionalProblem::setCondition(double **array, quint8 row)
{
    QLabel *label;
    QString str;
    QLayoutItem *item;
    m_row = row;
    m_array = array;
    //free memory
    while((item = ui->functionLayout->takeAt(0)) != 0)
    {
        delete item->widget();
    }
    while((item = ui->exprLayout->takeAt(0)) != 0)
    {
        delete item->widget();
    }
    m_signs.clear();
    //processing array
    //function
    label = new QLabel();
    str += "Z = ";
    if(m_array[m_row - 1][0] == 1)
        str += "X<span style=\" vertical-align:sub;\">1</span>";
    if(m_array[m_row - 1][0] != 0 && m_array[m_row - 1][0] != 1)
        str += QString::number(m_array[m_row - 1][0]) + "X<span style=\" vertical-align:sub;\">1</span>";
    if(m_array[m_row - 1][1] == 1)
        str += " + X<span style=\" vertical-align:sub;\">2</span>";
    if(m_array[m_row - 1][1] < 0)
        str += " - " + QString::number(m_array[m_row - 1][1]) + "X<span style=\" vertical-align:sub;\">2</span>";
    if(m_array[m_row - 1][1] > 0)
        str += " + " + QString::number(m_array[m_row - 1][1]) + "X<span style=\" vertical-align:sub;\">2</span>";
    if(m_array[m_row - 1][2] != 0)
        str += " + " + QString::number(m_array[m_row - 1][2]);
    label->setText(str);
    ui->functionLayout->addWidget(label);
    //expressions
    for(quint8 i = 0; i < m_row - 1; i++)
    {
        label = new QLabel();
        str.clear();
        //X1
        if(m_array[i][0] == 1)
            str += "X<span style=\" vertical-align:sub;\">1</span>";
        if(m_array[i][0] != 1 && m_array[i][0] != 0)
            str += QString::number(m_array[i][0]) + "X<span style=\" vertical-align:sub;\">1</span>";
        //X2
        if(m_array[i][1] == 1)
            str += " + X<span style=\" vertical-align:sub;\">1</span>";
        if(m_array[i][1] < 0)
            str += " - " + QString::number(m_array[i][1] * -1) + "X<span style=\" vertical-align:sub;\">2</span>";
        if(m_array[i][1] > 0)
            str += " + " + QString::number(m_array[i][1]) + "X<span style=\" vertical-align:sub;\">2</span>";
        //C
        str += " >= " + QString::number(m_array[i][2] * -1);
        label->setText(str);
        ui->exprLayout->addWidget(label);
    }
}

void TwoDimensionalProblem::on_backButton_clicked()
{
    emit back();
}

void TwoDimensionalProblem::on_nextButton_clicked()
{
    emit next();
    emit result(m_array, m_row);
}
