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

void MinMaxIntercectCheckDialog::check()
{
    QString result;
    if(ui->minXLineEdit->text().toInt() != m_correctMinPoint.x() ||
            ui->minYLineEdit->text().toInt() != m_correctMinPoint.y())
    {
        emit userError();
        result += QString::fromLocal8Bit("Ошибка в координатах %1 точки! Правильно: (%2:%3)")
                .arg("min")
                .arg(QString::number(m_correctMinPoint.x()))
                .arg(QString::number(m_correctMinPoint.y()));
    }
    if(ui->maxXLineEdit->text().toInt() != m_correctMaxPoint.x() ||
            ui->maxYLineEdit->text().toInt() != m_correctMaxPoint.y())
    {
        emit userError();
        result += "\n";
        result += QString::fromLocal8Bit("Ошибка в координатах %1 точки! Правильно: (%2:%3)")
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
    m_correctMinPoint = minPoint.toPoint();
    m_correctMaxPoint = maxPoint.toPoint();
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
