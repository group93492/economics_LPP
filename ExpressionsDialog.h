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
    QComboBox* signComboBox(); // + - * /
    QComboBox* signComboBox2(); // = > < >= <=
    QVBoxLayout *m_exprLayout; //layout for expressions
    qint8 **m_varArray; //coefficients array
    QLineEdit **m_wArray; //widgets of coefficients array
    qint8 *m_constArray; //constants array
    QLineEdit *m_wConstArray; //widgets of constants array
    QIntValidator *m_intValidator;
    qint8 m_row; // m_varArray rows
    qint8 m_col; // m_varArray columns
public slots:
    void setCondition(int var, int expr);

private slots:
    void on_nextButton_clicked();

    void on_backButton_clicked();

signals:
    void next();
    void back();
    void result(qint8** varArray, qint8 *constArray, qint8 row, qint8 col);
};

#endif // EXPRESSIONSDIALOG_H
