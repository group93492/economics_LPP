#include "ConditionDialog.h"
#include "ui_ConditionDialog.h"

ConditionDialog::ConditionDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionDialog)
{
    ui->setupUi(this);
}

ConditionDialog::~ConditionDialog()
{
    delete ui;
}

void ConditionDialog::on_pushButton_clicked()
{
    emit result(ui->varSpinBox->value(), ui->exprSpinBox->value());
    emit next();
}
