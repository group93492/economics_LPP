#include "GraphDialog1.h"
#include "ui_GraphDialog1.h"

GraphDialog1::GraphDialog1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDialog1)
{
    ui->setupUi(this);
    m_drawDialog = new DrawDialog();
    ui->paintLayout->insertWidget(1, m_drawDialog);
    ui->scaleSpinBox->setFocus();
}

GraphDialog1::~GraphDialog1()
{
    delete ui;
}

void GraphDialog1::setCondition(double **array, quint8 rows)
{
    m_array = array;
    m_rows = rows;
    ui->scaleSpinBox->setValue(1);
    m_drawDialog->drawTheProblem(array, rows);
}

void GraphDialog1::on_nextButton_clicked()
{
    if(!m_drawDialog->check())
    {
        emit userError();
        QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"),
                                    QString::fromLocal8Bit("Не верно выбрана область!"),
                                    QMessageBox::Ok);
    }
    else
    {
        emit result(m_array, m_rows);
        emit next();
    }
}

void GraphDialog1::on_backButton_clicked()
{
    emit back();
}

void GraphDialog1::on_scaleSpinBox_valueChanged(double arg1)
{
    m_drawDialog->setScale(qreal(arg1));
}
