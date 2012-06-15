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
    TwoDimensionalProblem *ddialog = new TwoDimensionalProblem();
    GraphDialog1 *edialog = new GraphDialog1();
    GraphDialog2 *fdialog = new GraphDialog2();
    MinMaxIntercectCheckDialog *gdialog = new MinMaxIntercectCheckDialog();
    FinishDialog *zdialog = new FinishDialog();
    //add widgets
    ui->stackedWidget->addWidget(adialog);
    ui->stackedWidget->addWidget(bdialog);
    ui->stackedWidget->addWidget(cdialog);
    ui->stackedWidget->addWidget(ddialog);
    ui->stackedWidget->addWidget(edialog);
    ui->stackedWidget->addWidget(fdialog);
    ui->stackedWidget->addWidget(gdialog);
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
    connect(cdialog, SIGNAL(result(double**,quint8)), ddialog, SLOT(setCondition(double**,quint8)));

    connect(ddialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(ddialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(ddialog, SIGNAL(result(double**,quint8)), edialog, SLOT(setCondition(double**,quint8)));

    connect(edialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(edialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(edialog, SIGNAL(result(double**,quint8)), fdialog, SLOT(setCondition(double**,quint8)));
    connect(edialog, SIGNAL(userError()), zdialog, SLOT(addUserError()));

    connect(fdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(fdialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(fdialog, SIGNAL(result(QPointF,QPointF)), gdialog, SLOT(getResult(QPointF,QPointF)));
    connect(fdialog, SIGNAL(userError()), zdialog, SLOT(addUserError()));

    connect(gdialog, SIGNAL(next()), this, SLOT(nextWidget()));
    connect(gdialog, SIGNAL(back()), this, SLOT(previousWidget()));
    connect(gdialog, SIGNAL(userError()), zdialog, SLOT(addUserError()));

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
