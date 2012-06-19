#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QWidget>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();
    
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::PasswordDialog *ui;

signals:
    void accept();
};

#endif // PASSWORDDIALOG_H
