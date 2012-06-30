#include "GraphDialog2.h"
#include "ui_GraphDialog2.h"

GraphDialog2::GraphDialog2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDialog2)
{
    ui->setupUi(this);
    m_drawDialog = new MinMaxDrawDialog();
    ui->paintLayout->insertWidget(1, m_drawDialog);
    connect(m_drawDialog, SIGNAL(userAnswerFalse()), SLOT(replyUserError()));
    connect(m_drawDialog, SIGNAL(result(QPointF,QPointF,qreal,qreal)), this, SLOT(replyResult(QPointF,QPointF,qreal,qreal)));
//    ui->scaleSpinBox->setFocus();
}

GraphDialog2::~GraphDialog2()
{
    delete ui;
}

void GraphDialog2::showHint()
{
    m_drawDialog->setCorrectAnswer();
}

void GraphDialog2::setCondition(double **array, quint8 rows)
{
    ui->scaleSpinBox->setValue(1);
    m_drawDialog->drawTheProblem(array, rows);
}

void GraphDialog2::replyUserError()
{
    emit userError();
}

void GraphDialog2::replyResult(QPointF min, QPointF max, qreal minZ, qreal maxZ)
{
    emit result(min, max, minZ, maxZ);
}

void GraphDialog2::on_nextButton_clicked()
{
    if(!m_drawDialog->check())
        QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"),
                                    QString::fromLocal8Bit("Не верно выбран минимум или максимум функции Z!"),
                                    QMessageBox::Ok);
    else
        emit next();
}

void GraphDialog2::on_backButton_clicked()
{
    emit back();
}

void GraphDialog2::on_scaleSpinBox_valueChanged(double arg1)
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
