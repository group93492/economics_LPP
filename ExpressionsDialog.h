#ifndef EXPRESSIONSDIALOG_H
#define EXPRESSIONSDIALOG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

namespace Ui {
class ExpressionsDialog;
}

class ExpressionsDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExpressionsDialog(QWidget *parent = 0);
    ~ExpressionsDialog();
    
private:
    Ui::ExpressionsDialog *ui;
    QComboBox* signComboBox(); // + - * /
    QComboBox* signComboBox2(); // = > < >= <=

public slots:
    void setCondtion(int var, int expr);

};

#endif // EXPRESSIONSDIALOG_H
