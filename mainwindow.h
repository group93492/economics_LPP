#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConditionDialog.h"
#include "ExpressionsDialog.h"
#include "SolvedSystemDialog.h"
#include "FinishDialog.h"
#include "TwoDimensionalProblem.h"
#include "drawdialog.h"
#include "minmaxdrawdialog.h"

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

private slots:
    void nextWidget();
    void previousWidget();
    void on_quitButton_clicked();
    void on_resetButton_clicked();
};

#endif // MAINWINDOW_H
