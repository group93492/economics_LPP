#include "FinishDialog.h"
#include "ui_FinishDialog.h"

FinishDialog::FinishDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishDialog)
{
    ui->setupUi(this);
    reset();
}

FinishDialog::~FinishDialog()
{
    delete ui;
}

void FinishDialog::addUserError()
{
    m_error++;
    ui->infLabel->setText(QString::fromLocal8Bit("Ошибок допущено: ") + QString::number(m_error));
}

void FinishDialog::reset()
{
    m_error = 0;
    ui->infLabel->setText(QString::fromLocal8Bit("Ошибок допущено: ") + QString::number(m_error));
}

void FinishDialog::setAnswer(qreal x1, qreal y1, qreal x2, qreal y2, qreal minZ, qreal maxZ)
{
    ui->minLabel->setText(QString::fromLocal8Bit("Z<span style=\" vertical-align:sub;\">%1</span> = %2 в точке (%3:%4)")
                          .arg("min")
                          .arg(QString::number(minZ))
                          .arg(QString::number(x1))
                          .arg(QString::number(y1)));
    ui->maxLabel->setText(QString::fromLocal8Bit("Z<span style=\" vertical-align:sub;\">%1</span> = %2 в точке (%3:%4)")
                          .arg("max")
                          .arg(QString::number(maxZ))
                          .arg(QString::number(x2))
                          .arg(QString::number(y2)));
}
