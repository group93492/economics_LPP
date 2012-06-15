#ifndef MINMAXINTERCECTCHECKDIALOG_H
#define MINMAXINTERCECTCHECKDIALOG_H

#include <QWidget>
#include <QMessageBox>
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
    void check();
    
private:
    Ui::MinMaxIntercectCheckDialog *ui;

signals:
    void userError();
    void back();
    void next();

public slots:
    void getResult(QPointF minPoint, QPointF maxPoint);

private slots:
    void on_nextPushButton_clicked();
    void on_previousPushButton_clicked();
};

#endif // MINMAXINTERCECTCHECKDIALOG_H
