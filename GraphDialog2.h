#ifndef GRAPHDIALOG2_H
#define GRAPHDIALOG2_H

#include <QWidget>
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
    void replyUserError();

signals:
    void next();
    void back();
    void userError();

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();
    void on_skaleSpinBox_valueChanged(double arg1);
    void on_nextMinButton_clicked();
    void on_nextMaxButton_clicked();
};

#endif // GRAPHDIALOG2_H
