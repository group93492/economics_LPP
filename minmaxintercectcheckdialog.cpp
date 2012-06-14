#include "minmaxintercectcheckdialog.h"
#include "ui_minmaxintercectcheckdialog.h"

MinMaxIntercectCheckDialog::MinMaxIntercectCheckDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinMaxIntercectCheckDialog)
{
    ui->setupUi(this);
    m_intValidator = new QIntValidator();
    ui->minXLineEdit->setValidator(m_intValidator);
    ui->minYLineEdit->setValidator(m_intValidator);
    ui->maxXLineEdit->setValidator(m_intValidator);
    ui->maxYLineEdit->setValidator(m_intValidator);
}

MinMaxIntercectCheckDialog::~MinMaxIntercectCheckDialog()
{
    delete ui;
}
