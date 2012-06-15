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
    connect(ui->nextPushButton, SIGNAL(clicked()), this, SLOT(next()));
}

MinMaxIntercectCheckDialog::~MinMaxIntercectCheckDialog()
{
    delete ui;
}

void MinMaxIntercectCheckDialog::next()
{
    if(ui->minXLineEdit->text().toInt() != m_correctMinPoint.x() ||
            ui->minYLineEdit->text().toInt() != m_correctMinPoint.y())
        emit userAnswerFalse();
    if(ui->maxXLineEdit->text().toInt() != m_correctMaxPoint.x() ||
            ui->maxYLineEdit->text().toInt() != m_correctMaxPoint.y())
        emit userAnswerFalse();
}

void MinMaxIntercectCheckDialog::getResult(QPointF minPoint, QPointF maxPoint)
{
    m_correctMinPoint = minPoint.toPoint();
    m_correctMaxPoint = maxPoint.toPoint();
}
