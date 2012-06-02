#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //create widgets
    ConditionDialog *adialog = new ConditionDialog();
    ExpressionsDialog *bdialog = new ExpressionsDialog();
    SolvedSystemDialog *cdialog = new SolvedSystemDialog();
    FinishDialog *zdialog = new FinishDialog();
    //add widgets
    ui->stackedWidget->addWidget(adialog);
    ui->stackedWidget->addWidget(bdialog);
    ui->stackedWidget->addWidget(cdialog);
    ui->stackedWidget->addWidget(zdialog);
    //connect
    connect(adialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(adialog, SIGNAL(result(int,int)), bdialog, SLOT(setCondition(int,int)));

    connect(bdialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(bdialog, SIGNAL(result(double*,double**,double*,quint8,quint8)), cdialog, SLOT(setCondition(double*,double**,double*,quint8,quint8)));
    connect(bdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(bdialog, SIGNAL(userError()), zdialog, SLOT(addUserError()));

    connect(cdialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(cdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(cdialog, SIGNAL(userError()), zdialog, SLOT(addUserError()));

    connect(ui->resetButton, SIGNAL(clicked()), zdialog, SLOT(reset()));
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

void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_resetButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
