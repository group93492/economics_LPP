#include "SolvedSystemDialog.h"
#include "ui_SolvedSystemDialog.h"
#include <QDebug>

SolvedSystemDialog::SolvedSystemDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolvedSystemDialog)
{
    ui->setupUi(this);
    m_intValidator = new QIntValidator;
}

SolvedSystemDialog::~SolvedSystemDialog()
{
    delete ui;
}

void SolvedSystemDialog::setCondition(double *genExprArray, double **varArray, double *constArray, quint8 row, quint8 col)
{
    m_row = row;
    m_col = col;
    //for debug
//    qDebug() << "row = " << row << " col = " << col;
//    qDebug() << "Array:";
//    for(quint8 i = 0; i < row; i++)
//    {
//        for(quint8 j = 0; j < col; j++)
//            qDebug() << varArray[i][j];
//        qDebug() << "endline";
//    }
//    qDebug() << "Function array:";
//    for(quint8 i = 0; i < col + 1; i++)
//    {
//        qDebug() << genExprArray[i];
//    }
//    qDebug() << "Const array:";
//    for(quint8 i = 0; i < row; i++)
//        qDebug() << constArray[i];
    freeMemory();
    SystemLinearEquations solve(genExprArray, varArray, constArray, col, row);
    m_solvedArray = solve.Solve();
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
    for(quint8 i = 0; i < m_col + 1; i++)
    {
        m_wArray.append(new QVector<QLineEdit*>); //lineedits for coefficents
        for(quint8 j = 0; j < 3; j++)
        {
            lineEdit = new QLineEdit();
            lineEdit->setAlignment(Qt::AlignRight);
            //lineEdit->setValidator(m_intValidator); //for debug!
            lineEdit->setText(QString::number(m_solvedArray[i][j])); //temporary, for debug
            m_wArray[i]->append(lineEdit);
        }
        //lineEdit = new QLineEdit();
        //lineEdit->setValidator(m_intValidator);
    }
    for(quint8 i = 0; i < m_col; i++)  //layouts
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
        ui->functionLayout->addWidget(m_wArray.value(m_col)->value(i));
        ui->functionLayout->addWidget(label);
        label = new QLabel(" + ");
        label->setMaximumWidth(22);
        ui->functionLayout->addWidget(label);
    }
    ui->functionLayout->addWidget(m_wArray.value(m_col)->value(2));
    for(quint8 i = 0; i < m_col; i++)
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

void SolvedSystemDialog::on_backButton_clicked()
{
    emit back();
}

void SolvedSystemDialog::on_nextButton_clicked()
{
    emit next();
}
