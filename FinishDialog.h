#ifndef FINISHDIALOG_H
#define FINISHDIALOG_H

#include <QWidget>

namespace Ui {
class FinishDialog;
}

class FinishDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit FinishDialog(QWidget *parent = 0);
    ~FinishDialog();
    
private:
    Ui::FinishDialog *ui;
    quint8 m_error;

public slots:
    void addUserError();
    void reset();
    void setAnswer(qreal x1, qreal y1, qreal x2, qreal y2, qreal minZ, qreal maxZ);
};

#endif // FINISHDIALOG_H
