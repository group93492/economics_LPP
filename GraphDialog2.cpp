#include "GraphDialog2.h"
#include "ui_GraphDialog2.h"

GraphDialog2::GraphDialog2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDialog2)
{
    ui->setupUi(this);
    m_drawDialog = new MinMaxDrawDialog();
    ui->paintLayout->addWidget(m_drawDialog);
    connect(m_drawDialog, SIGNAL(userAnswerFalse()), SLOT(replyUserError()));
}

GraphDialog2::~GraphDialog2()
{
    delete ui;
}

void GraphDialog2::setCondition(double **array, quint8 rows)
{
    m_drawDialog->drawTheProblem(array, rows);
}

void GraphDialog2::replyUserError()
{
    emit userError();
}

void GraphDialog2::on_nextButton_clicked()
{
    emit next();
}

void GraphDialog2::on_backButton_clicked()
{
    emit back();
}

void GraphDialog2::on_skaleSpinBox_valueChanged(double arg1)
{
    m_drawDialog->setScale(qreal(arg1));
}

void GraphDialog2::on_nextMinButton_clicked()
{
    m_drawDialog->nextMin();
}

void GraphDialog2::on_nextMaxButton_clicked()
{
    m_drawDialog->nextMax();
}
