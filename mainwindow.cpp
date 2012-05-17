#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConditionDialog *cdialog = new ConditionDialog();
    ExpressionsDialog *edialog = new ExpressionsDialog();
    ui->stackedWidget->addWidget(cdialog);
    ui->stackedWidget->addWidget(edialog);
    connect(cdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(cdialog, SIGNAL(result(int,int)), edialog, SLOT(setCondtion(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextWidget()
{
    if(ui->stackedWidget->currentIndex() + 1 <= ui->stackedWidget->count())
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
}
