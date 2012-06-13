#include "GraphDialog1.h"
#include "ui_GraphDialog1.h"

GraphDialog1::GraphDialog1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDialog1)
{
    ui->setupUi(this);
    m_drawDialog = new DrawDialog();
    ui->generalLayout->addWidget(m_drawDialog);
    connect(m_drawDialog, SIGNAL(userAnswerFalse()), this, SLOT(replyUserError()));
}

GraphDialog1::~GraphDialog1()
{
    delete ui;
}

void GraphDialog1::replyUserError()
{
    emit userError();
}

void GraphDialog1::setCondition(double **array, quint8 rows)
{
    m_array = array;
    m_rows = rows;
    m_drawDialog->drawTheProblem(array, rows);
}

void GraphDialog1::on_nextButton_clicked()
{
    emit result(m_array, m_rows);
    emit next();
}

void GraphDialog1::on_backButton_clicked()
{
    emit back();
}

void GraphDialog1::on_skaleSpinBox_valueChanged(double arg1)
{
    m_drawDialog->setScale(qreal(arg1));
}
