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
    SolvedSystemDialog *sdialog = new SolvedSystemDialog();
    ui->stackedWidget->addWidget(cdialog);
    ui->stackedWidget->addWidget(edialog);
    ui->stackedWidget->addWidget(sdialog);
    connect(cdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(cdialog, SIGNAL(result(int,int)), edialog, SLOT(setCondition(int,int)));
    connect(edialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(edialog, SIGNAL(result(double*,double**,double*,quint8,quint8)), sdialog, SLOT(setCondition(double*,double**,double*,quint8,quint8)));
    connect(edialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(sdialog, SIGNAL(back()), this, SLOT(previousWidget()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextWidget()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
}

void MainWindow::previousWidget()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
}
