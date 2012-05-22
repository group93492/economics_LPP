#include "ExpressionsDialog.h"
#include "ui_ExpressionsDialog.h"
#include <QDebug>

ExpressionsDialog::ExpressionsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExpressionsDialog)
{
    ui->setupUi(this);
    m_exprLayout = NULL;
    m_varArray = NULL;
    m_wArray = NULL;
    m_constArray = NULL;
    m_wConstArray = NULL;
    m_intValidator = new QIntValidator();
}

ExpressionsDialog::~ExpressionsDialog()
{
    if(m_wArray != NULL)
    {
        for(quint8 i = 0; i < m_row; i++)
        {
            delete [] m_wArray[i];
        }
        delete [] m_wArray;
    }
    if(m_wConstArray != NULL)
        delete [] m_wConstArray;
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
    m_colOld = m_col;
    m_rowOld = m_row;
    m_col = var;
    m_row = expr;
    //free memory
    if(m_wArray != NULL)
    {
        for(quint8 i = 0; i < m_rowOld; i++)
        {
            delete [] m_wArray[i];
        }
        delete [] m_wArray;
    }
    if(m_wConstArray != NULL)
        delete [] m_wConstArray;
    for(quint8 i = 0; i < m_labels.size(); i++)
    {
        delete m_labels.value(i);
    }
    m_labels.clear();
    for(quint8 i = 0; i < m_signs.size(); i++)
    {
        delete m_signs.value(i);
    }
    m_signs.clear();
    QLayoutItem *item;
    if(m_exprLayout != NULL)
    {
        while((item = m_exprLayout->takeAt(0)) != 0)
            delete item;
        delete m_exprLayout;
    }
    //allocate memory
    m_wConstArray = new QLineEdit[m_row];
    m_wArray = new QLineEdit*[m_row];
    for(quint8 i = 0; i < m_row; i++)
    {
        m_wArray[i] = new QLineEdit[m_col];
        for(quint8 j = 0; j < m_col; j++)
        {
            m_wArray[i][j].setAlignment(Qt::AlignRight);
            m_wArray[i][j].setValidator(m_intValidator);
        }
        m_wConstArray[i].setAlignment(Qt::AlignRight);
        m_wConstArray[i].setValidator(m_intValidator);
    }
    //
    QLabel *label;
    m_exprLayout = new QVBoxLayout();
    ui->verticalLayout->insertLayout(0, m_exprLayout);
    QHBoxLayout *layout;
    //create general expression

    //show widgets
    for(quint8 i = 0; i < m_row; i++)
    {
        layout = new QHBoxLayout();
        for(quint8 j = 0; j < m_col; j++)
        {
            layout->addWidget(&m_wArray[i][j]);
            label = new QLabel("X" + QString::number(j + 1));
            label->setMaximumWidth(22);
            layout->addWidget(label);
            m_labels.append(label);
        }
        layout->addWidget(signComboBox());
        layout->addWidget(&m_wConstArray[i]);
        m_exprLayout->addLayout(layout);
    }
}

void ExpressionsDialog::on_nextButton_clicked()
{
    //free memory
    if(m_varArray != NULL)
    {
        for(quint8 i = 0; i < m_rowOld; i++)
        {
            delete [] m_varArray[i];
        }
        delete [] m_varArray;
    }
    if(m_constArray != NULL)
        delete [] m_constArray;
    //allocate memory
    quint8 extVars = 0;
    for(quint8 i = 0; i < m_row; i++)
        if(m_signs.value(i)->currentText() != "=")
            extVars++;
    m_varArray = new qint8*[m_row + extVars];
    for(quint8 i = 0; i < m_row; i++)
    {
        m_varArray[i] = new qint8[m_col];
    }
    m_constArray = new qint8[m_row];
    //obtaining values from widgets
    quint8 extVarsCounter = 0;
    for(quint8 j = 0; j < m_row; j++)
    {
        for(quint8 k = 0; k < m_col; k++)
        {
            m_varArray[j][k] = m_wArray[j][k].text().toInt();
        }
        if(m_signs.value(j)->currentText() == ">" || m_signs.value(j)->currentText() == ">=")
        {
            for(quint8 i = 0; i < m_row; i++)
                if(i == j)
                    m_varArray[i][m_col + extVarsCounter] = -1;
                else
                    m_varArray[i][m_col + extVarsCounter] = 0;
            extVarsCounter++;
        }
        if(m_signs.value(j)->currentText() == "<" || m_signs.value(j)->currentText() == "<=")
        {
            for(quint8 i = 0; i < m_row; i++)
            {
                if(i == j)
                    m_varArray[i][m_col + extVarsCounter] = 1;
                else
                    m_varArray[i][m_col + extVarsCounter] = 0;

            }
            extVarsCounter++;
        }
        m_constArray[j] = m_wConstArray[j].text().toInt();
    }
    emit result(m_varArray, m_constArray, m_row, m_col);
    emit next();
    for(quint8 i = 0; i < m_row; i++)
    {
        qDebug() << "endline";
        for(quint8 j = 0; j < m_col + extVars; j++)
            qDebug() << m_varArray[i][j];
    }
}

void ExpressionsDialog::on_backButton_clicked()
{
    emit back();
}
