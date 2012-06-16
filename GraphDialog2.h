#ifndef GRAPHDIALOG2_H
#define GRAPHDIALOG2_H

#include <QWidget>
#include <QMessageBox>
#include "minmaxdrawdialog.h"

namespace Ui {
class GraphDialog2;
}

class GraphDialog2 : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphDialog2(QWidget *parent = 0);
    ~GraphDialog2();
    
private:
    Ui::GraphDialog2 *ui;
    MinMaxDrawDialog *m_drawDialog;

public slots:
    void setCondition(double **array, quint8 rows);

signals:
    void next();
    void back();
    void result(QPointF, QPointF);
    void userError();

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();
    void on_scaleSpinBox_valueChanged(double arg1);
    void on_nextMinButton_clicked();
    void on_nextMaxButton_clicked();
    void replyUserError();
    void replyResult(QPointF min, QPointF max);
};

#endif // GRAPHDIALOG2_H
