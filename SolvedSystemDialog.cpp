#include "SolvedSystemDialog.h"
#include "ui_SolvedSystemDialog.h"

SolvedSystemDialog::SolvedSystemDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolvedSystemDialog)
{
    ui->setupUi(this);
    m_doubleValidator = new QDoubleValidator;
}

SolvedSystemDialog::~SolvedSystemDialog()
{
    delete ui;
}

void SolvedSystemDialog::setCondition(double *genExprArray, double **varArray, double *constArray, quint8 row, quint8 col)
{
    m_row = row;
    m_col = col;
    freeMemory();
    SystemLinearEquations solve(genExprArray, varArray, constArray, col, row);
    m_solvedArray = solve.Solve();
    for(quint8 i = 0; i <= m_row; i++)
        for(quint8 j = 0; j < 3; j++)
            m_solvedArray[i][j] = _round(m_solvedArray[i][j]);
    if(!m_solvedArray)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"),
                                 QString::fromLocal8Bit("Эту систему нельзя решить!"),
                                    QMessageBox::Ok);
        emit back();
        return;
    }
    allocateMemory();
    placeWidgets();
}

void SolvedSystemDialog::freeMemory()
{
    QLayoutItem *item;
    while((item = ui->functionLayout->takeAt(0)) != 0)
    {
        delete item->widget();
    }
    for(quint8 i = 0; i < m_layouts.size(); i++)
    {
        while((item = m_layouts.value(i)->takeAt(0)) != 0)
            delete item->widget();
    }
    while((item = ui->exprLayout->takeAt(0)) != 0)
        delete item->layout();
    for(quint8 i = 0; i < m_wArray.size(); i++)
    {
        delete m_wArray.value(i);
    }
    m_wArray.clear();
    m_layouts.clear();
}

void SolvedSystemDialog::allocateMemory()
{
    QLineEdit *lineEdit;
    for(quint8 i = 0; i < m_row + 1; i++)
    {
        m_wArray.append(new QVector<QLineEdit*>); //lineedits for coefficents
        for(quint8 j = 0; j < 3; j++)
        {
            lineEdit = new QLineEdit();
            lineEdit->setAlignment(Qt::AlignRight);
            lineEdit->setValidator(m_doubleValidator);
            m_wArray[i]->append(lineEdit);
        }
    }
    for(quint8 i = 0; i < m_row; i++)  //layouts
    {
        m_layouts.append(new QHBoxLayout());
    }
}

void SolvedSystemDialog::placeWidgets()
{
    QLabel *label;
    ui->functionLayout->addWidget(new QLabel("Z = "));
    for(quint8 i = 0; i < 2; i++)
    {
        label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(i + 1)+ "</span>");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(m_wArray.value(m_row)->value(i));
        ui->functionLayout->addWidget(label);
        label = new QLabel(" + ");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(label);
    }
    ui->functionLayout->addWidget(m_wArray.value(m_row)->value(2));
    for(quint8 i = 0; i < m_row; i++)
    {
        label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(i + 3)+ "</span> = ");
        label->setMaximumWidth(22);
        m_layouts.value(i)->addWidget(label);
        for(quint8 j = 0; j < 2; j++)
        {
            label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(j + 1)+ "</span>");
            label->setMaximumWidth(22);
            m_layouts.value(i)->addWidget(m_wArray.value(i)->value(j));
            m_layouts.value(i)->addWidget(label);
            label = new QLabel(" + ");
            label->setMaximumWidth(22);
            m_layouts.value(i)->addWidget(label);
        }
        m_layouts.value(i)->addWidget(m_wArray.value(i)->value(2));
        ui->exprLayout->addLayout(m_layouts.value(i));
    }
}

void SolvedSystemDialog::check()
{
    QString result;
    for(quint8 i = 0; i < 3; i++)
        if(m_wArray.value(m_row)->value(i)->text().toDouble() != m_solvedArray[m_row][i])
        {
            if(i != 2)
                result += QString::fromLocal8Bit("Ошибка в %1, правильно: %2%3%4")
                        .arg(QString::fromLocal8Bit("функции"))
                        .arg(QString::number(m_solvedArray[m_row][i]))
                        .arg("X")
                        .arg("<span style=\" vertical-align:sub;\">" + QString::number(i + 1)+ "</span><br />");
            else
                result += QString::fromLocal8Bit("Ошибка в %1, правильно: %2%3%4")
                        .arg(QString::fromLocal8Bit("функции"))
                        .arg(QString::fromLocal8Bit("свободный член = "))
                        .arg(QString::number(m_solvedArray[m_row][i]))
                        .arg("<br />");
            emit userError();
        }
    for(quint8 i = 0; i < m_row; i++)
        for(quint8 j = 0; j < 3; j++)
        {
            if(m_wArray.value(i)->value(j)->text().toDouble() != m_solvedArray[i][j])
            {
                if(j != 2)
                    result += QString::fromLocal8Bit("Ошибка в %1, правильно: %2%3%4 \n")
                            .arg(QString::fromLocal8Bit("уравнении ") + QString::number(i + 1))
                            .arg(QString::number(m_solvedArray[i][j]))
                            .arg("X")
                            .arg("<span style=\" vertical-align:sub;\">" + QString::number(j + 1)+ "</span><br />");
                else
                    result += QString::fromLocal8Bit("Ошибка в %1,правильно: %2%3%4 \n")
                            .arg(QString::fromLocal8Bit("уравнении ") + QString::number(i + 1))
                            .arg(QString::fromLocal8Bit("свободный член = "))
                            .arg(QString::number(m_solvedArray[i][j]))
                            .arg("<br />");
                emit userError();
            }
        }
    if(result.isNull())
        return;
    if(QMessageBox::information(this, QString::fromLocal8Bit("Ошибки!"), result, QMessageBox::Ok) == QMessageBox::Ok)
        return;
}

double SolvedSystemDialog::_round(double n)
{
    n = n * 100;
    if(fmod(n, 1) * 10 > 4)
        return ceil(n) / 100;
    else
        return floor(n) / 100;
}

void SolvedSystemDialog::on_backButton_clicked()
{
    emit back();
}

void SolvedSystemDialog::on_nextButton_clicked()
{
    check();
    emit result(m_solvedArray, m_row + 1);
    emit next();
}
