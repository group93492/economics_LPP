#ifndef CONDITIONDIALOG_H
#define CONDITIONDIALOG_H

#include <QWidget>

namespace Ui {
class ConditionDialog;
}

class ConditionDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConditionDialog(QWidget *parent = 0);
    ~ConditionDialog();
    
private:
    Ui::ConditionDialog *ui;
};

#endif // CONDITIONDIALOG_H
