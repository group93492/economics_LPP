#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_okButton_clicked()
{
    if(ui->lineEdit->text().toInt() == QDate::currentDate().day() + QDate::currentDate().month())
    {
        hide();
        emit accept();
    }
    else
        QMessageBox::information(this, QString::fromLocal8Bit("Неверный пароль!"), QString::fromLocal8Bit("Неверный пароль"), QMessageBox::Ok);
    ui->lineEdit->clear();

}

void PasswordDialog::on_cancelButton_clicked()
{
    ui->lineEdit->clear();
    hide();
}
