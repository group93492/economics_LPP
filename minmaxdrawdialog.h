#ifndef MINMAXDRAWDIALOG_H
#define MINMAXDRAWDIALOG_H

#include <QWidget>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QLinkedList>
#include <drawdialog.h>

namespace Ui {
class MinMaxDrawDialog;
}

class MinMaxDrawDialog : public QWidget
{
    Q_OBJECT
    
private:
    Ui::MinMaxDrawDialog *ui;
    QSize m_size;//size of coordinate plain
    QSize m_windowSize;//size of paint window
    qreal m_scale;//size multiplayer for scaling
    QLinkedList<GraphicElement *> m_whatToDrawList;
    QColor m_textColor;
    QColor m_linesColor;
    static qreal toQtY(qreal Y);
    static qreal fromQtY(qreal Y);
    bool m_drawMinMax;//if we allready ready to draw min and max line
    QPair<QPointF, QPointF> m_minLineAnswer;
    QPair<QPointF, QPointF> m_maxLineAnswer;
    QPair<QPointF, QPointF> m_minLineUserAnswer;
    QPair<QPointF, QPointF> m_maxLineUserAnswer;
    QLinkedList<QPair<QPointF, QPointF> > m_solutionPoints;
    enum{
        indent = 20 //отступ в пикселях от краев окна для надписей и т.д.
    };
    void drawLine(const double x, const double y, const double c);
    QLineF getQLine(DrawLine Line);
    QLineF getOrdinaryLine(DrawLine Line);
    void findMin();
    void findMax();
    DrawLine Zfunction;
    QLinkedList<QPair<QPointF, QPointF> > findSolutionPolygon(QLinkedList<GraphicElement *> *drawList);
    void resetAllChanges();
    bool m_resetAllChanges;

public:
    explicit MinMaxDrawDialog(QWidget *parent = 0);
    explicit MinMaxDrawDialog(const QSize size, QWidget *parent = 0);
    ~MinMaxDrawDialog();
    void setScale(qreal scale);
    void setTextColor(QColor color);
    void setLinesColor(QColor color);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void drawTheProblem(double **array, quint8 rowsCount);
    void nextMin();
    void nextMax();
    void next();

signals:
    void userAnswerFalse();

private slots:

};

#endif // MINMAXDRAWDIALOG_H
