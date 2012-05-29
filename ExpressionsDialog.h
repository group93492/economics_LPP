#ifndef EXPRESSIONSDIALOG_H
#define EXPRESSIONSDIALOG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

namespace Ui {
class ExpressionsDialog;
}

class ExpressionsDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExpressionsDialog(QWidget *parent = 0);
    ~ExpressionsDialog();
    
private:
    Ui::ExpressionsDialog *ui;
    QComboBox* signComboBox(); // = > < >= <=
    QVector<QLineEdit*> m_wConstArray; //widgets of constants array
    QVector<QLineEdit*> m_genExprArray; // vector with coefficients for "Z = ..." expression
    QVector< QVector<QLineEdit*>* > m_wVarArray; // array with coefficentes for X
    QVector<QComboBox*> m_signs; //vector with QComboBoxes
    QVector<QHBoxLayout*> m_layouts;
    QIntValidator *m_intValidator;
    quint8 m_col; // m_varArray columns
    quint8 m_row; // m_varArray rows
    //methods:
    void freeMemory();
    void allocateMemory();
    void placeWidgets();

public slots:
    void setCondition(int var, int expr);

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();

signals:
    void next();
    void back();
    void result(double *genExprArray, double** varArray, double *constArray, quint8 row, quint8 col);
};

#endif // EXPRESSIONSDIALOG_H
