#ifndef MINMAXINTERCECTCHECKDIALOG_H
#define MINMAXINTERCECTCHECKDIALOG_H

#include <QWidget>
#include <QMessageBox>
#include <QDoubleValidator>

namespace Ui {
class MinMaxIntercectCheckDialog;
}

class MinMaxIntercectCheckDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit MinMaxIntercectCheckDialog(QWidget *parent = 0);
    ~MinMaxIntercectCheckDialog();
    QDoubleValidator *m_validator;
    QPointF m_correctMinPoint;
    QPointF m_correctMaxPoint;
    qreal m_minZ;
    qreal m_maxZ;
    bool check();
    
private:
    Ui::MinMaxIntercectCheckDialog *ui;

signals:
    void userError();
    void back();
    void next();
    void result(qreal x1, qreal y1, qreal x2, qreal y2, qreal minZ, qreal maxZ);

public slots:
    void getResult(QPointF minPoint, QPointF maxPoint, qreal minZ, qreal maxZ);

private slots:
    void on_nextPushButton_clicked();
    void on_previousPushButton_clicked();
};

#endif // MINMAXINTERCECTCHECKDIALOG_H
