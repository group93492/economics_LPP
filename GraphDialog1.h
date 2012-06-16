#ifndef GRAPHDIALOG1_H
#define GRAPHDIALOG1_H

#include <QWidget>
#include <drawdialog.h>
#include <QMessageBox>

namespace Ui {
class GraphDialog1;
}

class GraphDialog1 : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphDialog1(QWidget *parent = 0);
    ~GraphDialog1();
    void showHint();
    
private:
    Ui::GraphDialog1 *ui;
    DrawDialog *m_drawDialog;
    double **m_array;
    quint8 m_rows;

public slots:
    void setCondition(double **array, quint8 rows);

signals:
    void next();
    void result(double **array, quint8 rows);
    void back();
    void userError();

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();
    void on_scaleSpinBox_valueChanged(double arg1);
};

#endif // GRAPHDIALOG1_H
