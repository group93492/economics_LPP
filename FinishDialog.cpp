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
