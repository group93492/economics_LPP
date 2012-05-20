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
    list << "+" << "-" << "*" << "/";
    comboBox->addItems(list);
    comboBox->setMaximumWidth(35);
    return comboBox;
}

QComboBox *ExpressionsDialog::signComboBox2()
{
    QComboBox *comboBox = new QComboBox();
    QStringList list;
    list << "=" << "<" << "<=" << ">" << ">=";
    comboBox->addItems(list);
    comboBox->setMaximumWidth(35);
    return comboBox;
}

void ExpressionsDialog::setCondition(int var, int expr)
{
    //size
    m_col = var;
    m_row = expr;
    //free memory
    if(m_wArray != NULL)
    {
        for(quint8 i = 0; i < m_row; i++)
        {
            delete [] m_wArray[i];
        }
        delete [] m_wArray;
    }
    if(m_exprLayout != NULL)
        delete m_exprLayout;
    if(m_wConstArray != NULL)
        delete [] m_wConstArray;
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
    //show widgets
    for(quint8 i = 0; i < m_row; i++)
    {
        layout = new QHBoxLayout();
        for(quint8 j = 0; j < m_col; j++)
        {
            layout->addWidget(&m_wArray[i][j]);
            label = new QLabel("X" + QString::number(j + 1));
            label->setMaximumWidth(15);
            layout->addWidget(label);
            if(j != var - 1)
                layout->addWidget(signComboBox());
        }
        layout->addWidget(signComboBox2());
        layout->addWidget(&m_wConstArray[i]);
        m_exprLayout->addLayout(layout);
    }
}

void ExpressionsDialog::on_nextButton_clicked()
{
    //free memory
    if(m_varArray != NULL)
    {
        for(quint8 i = 0; i < m_row; i++)
        {
            delete [] m_varArray[i];
        }
        delete [] m_varArray;
    }
    if(m_constArray != NULL)
        delete [] m_constArray;
    //allocate memory
    m_varArray = new qint8*[m_row];
    for(quint8 i = 0; i < m_row; i++)
        m_varArray[i] = new qint8[m_col];
    m_constArray = new qint8[m_row];
    //obtaining values from widgets
    for(quint8 j = 0; j < m_row; j++)
    {
        for(quint8 k = 0; k < m_col; k++)
        {
            m_varArray[j][k] = m_wArray[j][k].text().toInt();
        }
        m_constArray[j] = m_wConstArray[j].text().toInt();
    }
}
