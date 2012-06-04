#ifndef TWODIMENSIONALPROBLEM_H
#define TWODIMENSIONALPROBLEM_H

#include <QWidget>

namespace Ui {
class TwoDimensionalProblem;
}

class TwoDimensionalProblem : public QWidget
{
    Q_OBJECT
    
public:
    explicit TwoDimensionalProblem(QWidget *parent = 0);
    ~TwoDimensionalProblem();
    
private:
    Ui::TwoDimensionalProblem *ui;
    QVector<QString> m_signs;
    quint8 m_row;
    double **m_array;

public slots:
    void setCondition(double **array, quint8 row);

signals:
    void next();
    void back();
    void result(double **array, quint8 row);

private slots:
    void on_backButton_clicked();
    void on_nextButton_clicked();
};

#endif // TWODIMENSIONALPROBLEM_H
