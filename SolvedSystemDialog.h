#ifndef SOLVEDSYSTEMDIALOG_H
#define SOLVEDSYSTEMDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QMessageBox>
#include "GaussJordano.h"

namespace Ui {
class SolvedSystemDialog;
}

class SolvedSystemDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit SolvedSystemDialog(QWidget *parent = 0);
    ~SolvedSystemDialog();

private:
    Ui::SolvedSystemDialog *ui;
    quint8 m_row;
    quint8 m_col;
    quint8 **m_array;
    QVector< QVector<QLineEdit*>* > m_wArray;
    QVector<QHBoxLayout*> m_layouts;
    QDoubleValidator *m_doubleValidator;
    double **m_solvedArray;
    //methods
    void freeMemory();
    void allocateMemory();
    void placeWidgets();
    void check();

public slots:
    void setCondition(double *genExprArray, double** varArray, double *constArray, quint8 row, quint8 col);

signals:
    void next();
    void back();
    void userError();

private slots:
    void on_backButton_clicked();
    void on_nextButton_clicked();
};

#endif // SOLVEDSYSTEMDIALOG_H
