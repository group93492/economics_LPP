#include "minmaxintercectcheckdialog.h"
#include "ui_minmaxintercectcheckdialog.h"
#include "SolvedSystemDialog.h"
#include <QDebug>

MinMaxIntercectCheckDialog::MinMaxIntercectCheckDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinMaxIntercectCheckDialog)
{
    ui->setupUi(this);
    m_validator = new QDoubleValidator();
    m_validator->setDecimals(2);//2 digits after separator
    ui->minXLineEdit->setValidator(m_validator);
    ui->minYLineEdit->setValidator(m_validator);
    ui->maxXLineEdit->setValidator(m_validator);
    ui->maxYLineEdit->setValidator(m_validator);
}

MinMaxIntercectCheckDialog::~MinMaxIntercectCheckDialog()
{
    delete ui;
}

bool MinMaxIntercectCheckDialog::check()
{
    QString result;
    if(ui->minXLineEdit->text().toDouble() != m_correctMinPoint.x() ||
            ui->minYLineEdit->text().toDouble() != m_correctMinPoint.y())
    {
        qDebug() << m_correctMinPoint.x() << m_correctMinPoint.y();
        qDebug() << m_correctMaxPoint.x() << m_correctMaxPoint.y();
        emit userError();
        result += QString::fromLocal8Bit("Неверные координаты %1 точки!")
                .arg("min");
    }
    if(ui->maxXLineEdit->text().toDouble() != m_correctMaxPoint.x() ||
            ui->maxYLineEdit->text().toDouble() != m_correctMaxPoint.y())
    {
        emit userError();
        result += "\n";
        result += QString::fromLocal8Bit("Неверные координаты %1 точки!")
                .arg("max");
    }
    if(result.isNull())
        return true;
    QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"), result, QMessageBox::Ok);
    return false;
}

void MinMaxIntercectCheckDialog::getResult(QPointF minPoint, QPointF maxPoint, qreal minZ, qreal maxZ)
{
    m_minZ = minZ;
    m_maxZ = maxZ;
    m_correctMinPoint = minPoint;
    m_correctMaxPoint = maxPoint;
    m_correctMinPoint.rx() = SolvedSystemDialog::_round(m_correctMinPoint.x());
    m_correctMinPoint.ry() = SolvedSystemDialog::_round(m_correctMinPoint.y());
    m_correctMaxPoint.rx() = SolvedSystemDialog::_round(m_correctMaxPoint.x());
    m_correctMaxPoint.ry() = SolvedSystemDialog::_round(m_correctMaxPoint.y());
}

void MinMaxIntercectCheckDialog::on_nextPushButton_clicked()
{
    if(check())
    {
        emit next();
        emit result(m_correctMinPoint.x(), m_correctMinPoint.y(), m_correctMaxPoint.x(), m_correctMaxPoint.y(),
                    m_minZ, m_maxZ);
    }
}

void MinMaxIntercectCheckDialog::on_previousPushButton_clicked()
{
    emit back();
}
