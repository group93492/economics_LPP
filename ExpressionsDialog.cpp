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

void ExpressionsDialog::setCondition(int var, int expr)
{
    //size
    m_col = var;
    m_row = expr;
    QLineEdit *lineEdit;
    QLabel *label;

    //free memory
    //delete all widgets
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

    //allocate memory
    for(quint8 i = 0; i < m_row; i++)  //layouts
    {
        m_layouts.append(new QHBoxLayout());
    }
    for(quint8 i = 0; i < m_col; i++) //lineedits for "Z = ..." function
    {
        m_genExprArray.append(new QLineEdit);
    }
    for(quint8 i = 0; i < m_row; i++)
    {
        m_wVarArray.append(new QVector<QLineEdit*>); //lineedits for coefficents
        for(quint8 j = 0; j < m_col; j++)
        {
            lineEdit = new QLineEdit();
            lineEdit->setValidator(m_intValidator);
            m_wVarArray[i]->append(lineEdit);
        }
        lineEdit = new QLineEdit();
        lineEdit->setValidator(m_intValidator);
        m_wConstArray.append(lineEdit); //and constants
    }

    //place widgets on layouts
    ui->functionLayout->addWidget(new QLabel("Z = "));
    for(quint8 i = 0; i < m_col; i++)
    {
        label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(i + 1)+ "</span>");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(m_genExprArray[i]);
        ui->functionLayout->addWidget(label);
    }
    for(quint8 i = 0; i < m_row; i++)
    {
        for(quint8 j = 0; j < m_col; j++)
        {
            label = new QLabel("X<span style=\" vertical-align:sub;\">" + QString::number(j + 1)+ "</span>");
            label->setMaximumWidth(22);
            m_layouts.value(i)->addWidget(m_wVarArray.value(i)->value(j));
            m_layouts.value(i)->addWidget(label);
        }
        m_layouts.value(i)->addWidget(signComboBox());
        m_layouts.value(i)->addWidget(m_wConstArray[i]);
    }

    //place layouts on exprLayout
    for(quint8 i = 0; i < m_row; i++)
    {
        ui->exprLayout->addLayout(m_layouts.value(i));
    }
}

void ExpressionsDialog::on_nextButton_clicked()
{
    //allocate memory
    qint8 extVars = 0;
    for(quint8 i = 0; i < m_signs.size(); i++)
        if(m_signs.value(i)->currentText() != "=")
            extVars++;
    qint8 **varArray = new qint8*[m_row];
    for(quint8 i = 0; i < m_row; i++)
        varArray[i] = new qint8[m_col + extVars];
    qint8 *genExprArray = new qint8[m_col];
    qint8 *constArray = new qint8[m_row];

    //obtaining values from widgets
    qint8 extVarsCounter = 0;
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
    for(quint8 i = 0; i < m_row; i++)
        genExprArray[i] = m_genExprArray.value(i)->text().toInt();
    for(quint8 i = 0; i < m_row; i++)
        constArray[i] = m_wConstArray.value(i)->text().toInt();
    emit next();
    //void result(qint8 *genExprArray, qint8** varArray, qint8 *constArray, qint8 row, qint8 col);
    //for debug
    qDebug() << "Array:";
    for(quint8 i = 0; i < m_row; i++)
    {
        for(quint8 j = 0; j < m_col + extVars; j++)
            qDebug() << varArray[i][j];
        qDebug() << "endline";
    }
    qDebug() << "Function array:";
    for(quint8 i = 0; i < m_col; i++)
    {
        qDebug() << genExprArray[i];
    }
    qDebug() << "Const array:";
    for(quint8 i = 0; i < m_row; i++)
        qDebug() << constArray[i];
}

void ExpressionsDialog::on_backButton_clicked()
{
    emit back();
}
