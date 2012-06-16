#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "ConditionDialog.h"
#include "ExpressionsDialog.h"
#include "SolvedSystemDialog.h"
#include "FinishDialog.h"
#include "TwoDimensionalProblem.h"
#include "GraphDialog1.h"
#include "GraphDialog2.h"
#include "minmaxintercectcheckdialog.h"
#include "AboutDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QTextEdit *m_textEdit;

private slots:
    void nextWidget();
    void previousWidget();
    void on_resetButton_clicked();
    void on_toolButton_clicked();
    void showHint();
};

#endif // MAINWINDOW_H
