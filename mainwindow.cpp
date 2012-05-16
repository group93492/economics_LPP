#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConditionDialog *dialog = new ConditionDialog();
    ui->stackedWidget->addWidget(dialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}
