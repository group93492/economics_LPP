#include "ExpressionsDialog.h"
#include "ui_ExpressionsDialog.h"

ExpressionsDialog::ExpressionsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExpressionsDialog)
{
    ui->setupUi(this);
    QVBoxLayout *genLayout = new QVBoxLayout();
    this->setLayout(genLayout);
}

ExpressionsDialog::~ExpressionsDialog()
{
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

void ExpressionsDialog::setCondtion(int var, int expr)
{
    delete this->layout();
    QLabel *label;
    QLineEdit *lineEdit;
    QVBoxLayout *genLayout = new QVBoxLayout();
    this->setLayout(genLayout);
    QHBoxLayout *layout;
    for(quint8 i = 0; i < expr; i++)
    {
        layout = new QHBoxLayout();
        for(quint8 j = 0; j < var; j++)
        {
            lineEdit = new QLineEdit();
            lineEdit->setMaximumWidth(30);
            layout->addWidget(lineEdit);
            label = new QLabel("X" + QString::number(j + 1));
            label->setMaximumWidth(15);
            layout->addWidget(label);
            layout->addWidget(signComboBox());
        }
        lineEdit = new QLineEdit();
        lineEdit->setMaximumWidth(25);
        layout->addWidget(lineEdit);
        layout->addWidget(signComboBox2());
        lineEdit = new QLineEdit();
        lineEdit->setMaximumWidth(30);
        layout->addWidget(lineEdit);
        //layout->addItem(new QSpacerItem(100,100));
        genLayout->addLayout(layout);
    }
}
