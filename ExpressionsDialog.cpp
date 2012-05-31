#include "ExpressionsDialog.h"
#include "ui_ExpressionsDialog.h"
#include <QDebug>

ExpressionsDialog::ExpressionsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExpressionsDialog)
{
    ui->setupUi(this);
    m_intValidator = new QIntValidator();
}

ExpressionsDialog::~ExpressionsDialog()
{
    delete ui;
}

QComboBox *ExpressionsDialog::signComboBox()
{
    QComboBox *comboBox = new QComboBox();
    QStringList list;
    list << "=" << "<" << "<=" << ">" << ">=";
    comboBox->addItems(list);
    comboBox->setMaximumWidth(35);
    m_signs.append(comboBox);
    return comboBox;
}

void ExpressionsDialog::freeMemory()
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
    //clear vectors
    m_wConstArray.clear();
    m_genExprArray.clear();
    m_signs.clear();
    m_layouts.clear();
    for(quint8 i = 0; i < m_wVarArray.size(); i++)
    {
        delete m_wVarArray.value(i);
    }
    m_wVarArray.clear();
}

void ExpressionsDialog::allocateMemory()
{
    QLineEdit *lineEdit;
    for(quint8 i = 0; i < m_row; i++)  //layouts
    {
        m_layouts.append(new QHBoxLayout());
    }
    for(quint8 i = 0; i <= m_col; i++) //lineedits for "Z = ..." function
    {
        lineEdit = new QLineEdit();
        lineEdit->setValidator(m_intValidator);
        lineEdit->setAlignment(Qt::AlignRight);
        m_genExprArray.append(lineEdit);
    }
    for(quint8 i = 0; i < m_row; i++)
    {
        m_wVarArray.append(new QVector<QLineEdit*>); //lineedits for coefficents
        for(quint8 j = 0; j < m_col; j++)
        {
            lineEdit = new QLineEdit();
            lineEdit->setValidator(m_intValidator);
            lineEdit->setAlignment(Qt::AlignRight);
            m_wVarArray[i]->append(lineEdit);
        }
        lineEdit = new QLineEdit();
        lineEdit->setValidator(m_intValidator);
        m_wConstArray.append(lineEdit); //and constants
    }
}

void ExpressionsDialog::placeWidgets()
{
    QLabel *label;
    ui->functionLayout->addWidget(new QLabel("Z = "));
    for(quint8 i = 0; i < m_col; i++)
    {
        ui->functionLayout->addWidget(m_genExprArray[i]);
        label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(i + 1)+ "</span>");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(label);
        label = new QLabel(" + ");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(label);
    }
    ui->functionLayout->addWidget(m_genExprArray[m_col]);
    for(quint8 i = 0; i < m_row; i++)
    {
        for(quint8 j = 0; j < m_col; j++)
        {
            m_layouts.value(i)->addWidget(m_wVarArray.value(i)->value(j));
            label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(j + 1)+ "</span>");
            label->setMaximumWidth(22);
            m_layouts.value(i)->addWidget(label);
            if(j != m_col - 1)
            {
                label = new QLabel(" + ");
                label->setMaximumWidth(22);
                m_layouts.value(i)->addWidget(label);
            }
        }
        m_layouts.value(i)->addWidget(signComboBox());
        m_layouts.value(i)->addWidget(m_wConstArray[i]);
    }
}

qint8 ExpressionsDialog::Rank(double **a, quint8 m, quint8 n)
{
        const double EPS = 1E-9;
        quint8 rank = std::max(m, n);
        QVector<char> line_used(m);
        for (quint8 i = 0; i < n; i++)
        {
            qint8 j;
            for (j = 0; j < m; j++)
                if (!line_used[j] && abs(a[j][i]) > EPS)
                    break;
            if (j == m)
                --rank;
            else
            {
                line_used[j] = true;
                for (quint8 p = i + 1; p < n; p++)
                    a[j][p] /= a[j][i];
                for (quint8 k = 0; k < m; k++)
                    if (k != j && abs (a[k][i]) > EPS)
                        for (quint8 p = i + 1; p < n; p++)
                            a[k][p] -= a[j][p] * a[k][i];
            }
        }
        return rank;
}

bool ExpressionsDialog::Check(double **a, quint8 m, quint8 n)
{
    if ((n - Rank(a, m, n)) <= 2)
        return true;
    return false;
}

void ExpressionsDialog::setCondition(int var, int expr)
{
    //size
    m_col = var;
    m_row = expr;
    freeMemory();
    allocateMemory();
    placeWidgets();
    //place layouts on exprLayout
    for(quint8 i = 0; i < m_row; i++)
    {
        ui->exprLayout->addLayout(m_layouts.value(i));
    }
}

void ExpressionsDialog::on_nextButton_clicked()
{
    //allocate memory
    quint8 extVars = 0;
    for(quint8 i = 0; i < m_signs.size(); i++)
        if(m_signs.value(i)->currentText() != "=")
            extVars++;
    double **varArray = new double*[m_row];
    for(quint8 i = 0; i < m_row; i++)
        varArray[i] = new double[m_col + extVars];
    double *genExprArray = new double[m_col + extVars  + 1];
    double *constArray = new double[m_row];

    //obtaining values from widgets
    quint8 extVarsCounter = 0;
    for(quint8 i = 0; i < m_row; i++)
    {
        for(quint8 j = 0; j < m_col; j++)
        {
            varArray[i][j] = m_wVarArray.value(i)->value(j)->text().toInt();
        }
        if(m_signs.value(i)->currentText() == ">" || m_signs.value(i)->currentText() == ">=")
        {
            for(quint8 k = 0; k < m_row; k++)
                if(k != i)
                    varArray[k][m_col + extVarsCounter] = 0;
                else
                    varArray[k][m_col + extVarsCounter] = -1;
            extVarsCounter++;
        }
        if(m_signs.value(i)->currentText() == "<" || m_signs.value(i)->currentText() == "<=")
        {
            for(quint8 k = 0; k < m_row; k++)
                if(k != i)
                    varArray[k][m_col + extVarsCounter] = 0;
                else
                    varArray[k][m_col + extVarsCounter] = 1;
            extVarsCounter++;
        }
    }
    for(quint8 i = 0; i < m_col + 1 + extVars; i++)
    {
        if(i < m_col)
            genExprArray[i] = m_genExprArray.value(i)->text().toInt();
        if(i == m_col)
            genExprArray[m_col + extVars] = m_genExprArray.value(i)->text().toInt();
        if(i >= m_col && i != m_col + extVars)
            genExprArray[i] = 0;
    }
    for(quint8 i = 0; i < m_row; i++)
        constArray[i] = m_wConstArray.value(i)->text().toInt();
    bool flag = Check(varArray, m_row, m_col + extVars);
    if(ui->checkBox->checkState() == Qt::Unchecked) //temporary
        return;
    if((ui->checkBox->checkState() == Qt::Checked && flag))
    {
        emit result(genExprArray, varArray, constArray, m_row, m_col + extVars);
        emit next();
    }
    else
    {
        QString str;
        if(flag)
            str = QString::fromLocal8Bit("Эту систему можно решить!");
        else
            str = QString::fromLocal8Bit("Эту систему нельзя решить");
        if(QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), str, QMessageBox::Ok) == QMessageBox::Ok)
            return;
    }
    //void result(qint8 *genExprArray, qint8** varArray, qint8 *constArray, qint8 row, qint8 col);
    //for debug
//    qDebug() << "Array:";
//    for(quint8 i = 0; i < m_row; i++)
//    {
//        for(quint8 j = 0; j < m_col + extVars; j++)
//            qDebug() << varArray[i][j];
//        qDebug() << "endline";
//    }
//    qDebug() << "Function array:";
//    for(quint8 i = 0; i < m_col + extVars + 1; i++)
//    {
//        qDebug() << genExprArray[i];
//    }
//    qDebug() << "Const array:";
//    for(quint8 i = 0; i < m_row; i++)
//        qDebug() << constArray[i];
}

void ExpressionsDialog::on_backButton_clicked()
{
    emit back();
}

