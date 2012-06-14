#ifndef MINMAXINTERCECTCHECKDIALOG_H
#define MINMAXINTERCECTCHECKDIALOG_H

#include <QWidget>
#include <QValidator>

namespace Ui {
class MinMaxIntercectCheckDialog;
}

class MinMaxIntercectCheckDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit MinMaxIntercectCheckDialog(QWidget *parent = 0);
    ~MinMaxIntercectCheckDialog();
    QValidator *m_intValidator;
    
private:
    Ui::MinMaxIntercectCheckDialog *ui;

public slots:


};

#endif // MINMAXINTERCECTCHECKDIALOG_H
