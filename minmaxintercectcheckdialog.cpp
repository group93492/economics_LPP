#include "minmaxintercectcheckdialog.h"
#include "ui_minmaxintercectcheckdialog.h"
#include "SolvedSystemDialog.h"

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

void MinMaxIntercectCheckDialog::check()
{
    QString result;
    if(ui->minXLineEdit->text().toFloat() != m_correctMinPoint.x() ||
            ui->minYLineEdit->text().toFloat() != m_correctMinPoint.y())
    {
        emit userError();
        result += QString::fromLocal8Bit("Неверные координаты %1 точки! Правильно: (%2:%3)")
                .arg("min")
                .arg(QString::number(m_correctMinPoint.x()))
                .arg(QString::number(m_correctMinPoint.y()));
    }
    if(ui->maxXLineEdit->text().toFloat() != m_correctMaxPoint.x() ||
            ui->maxYLineEdit->text().toFloat() != m_correctMaxPoint.y())
    {
        emit userError();
        result += "\n";
        result += QString::fromLocal8Bit("Неверные координаты %1 точки! Правильно: (%2:%3)")
                .arg("max")
                .arg(QString::number(m_correctMaxPoint.x()))
                .arg(QString::number(m_correctMaxPoint.y()));
    }
    if(result.isNull())
        return;
    if(QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"), result, QMessageBox::Ok) == QMessageBox::Ok)
        return;
}

void MinMaxIntercectCheckDialog::getResult(QPointF minPoint, QPointF maxPoint)
{
    m_correctMinPoint = minPoint;
    m_correctMaxPoint = maxPoint;
    m_correctMinPoint.rx() = SolvedSystemDialog::_round(m_correctMinPoint.x());
    m_correctMinPoint.ry() = SolvedSystemDialog::_round(m_correctMinPoint.y());
    m_correctMaxPoint.rx() = SolvedSystemDialog::_round(m_correctMaxPoint.x());
    m_correctMaxPoint.ry() = SolvedSystemDialog::_round(m_correctMaxPoint.y());
}

void MinMaxIntercectCheckDialog::on_nextPushButton_clicked()
{
    check();
    emit next();
}

void MinMaxIntercectCheckDialog::on_previousPushButton_clicked()
{
    emit back();
}
