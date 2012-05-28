#ifndef SOLVEDSYSTEMDIALOG_H
#define SOLVEDSYSTEMDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>
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
    
public slots:
    void setCondition(double *genExprArray, double** varArray, double *constArray, qint8 row, qint8 col);

private:
    Ui::SolvedSystemDialog *ui;
    quint8 m_row;
    quint8 m_col;
    quint8 **m_array;
    QVector< QVector<QLineEdit*>* > m_wArray;
    QVector<QHBoxLayout*> m_layouts;
    QIntValidator *m_intValidator;
};

#endif // SOLVEDSYSTEMDIALOG_H
