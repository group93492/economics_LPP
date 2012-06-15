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
    QPoint m_correctMinPoint;
    QPoint m_correctMaxPoint;
    
private:
    Ui::MinMaxIntercectCheckDialog *ui;

signals:
    void userAnswerFalse();

private slots:
    void getResult(QPointF minPoint, QPointF maxPoint);
    void next();

};

#endif // MINMAXINTERCECTCHECKDIALOG_H
