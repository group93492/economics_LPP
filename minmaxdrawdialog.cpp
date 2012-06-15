#include "minmaxdrawdialog.h"
#include "ui_minmaxdrawdialog.h"
#include <QStaticText>
#include <QLineF>
#include <qmath.h>

MinMaxDrawDialog::MinMaxDrawDialog(const QSize size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinMaxDrawDialog),
    m_windowSize(size),
    m_scale(1.0),
    m_textColor(Qt::red),
    m_linesColor(Qt::black),
    m_drawMinMax(false),
    m_resetAllChanges(false)
{
    ui->setupUi(this);
    m_size = m_windowSize / 2 - QSize(indent, indent);
}

MinMaxDrawDialog::MinMaxDrawDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinMaxDrawDialog),
    m_scale(1.0),
    m_textColor(Qt::red),
    m_linesColor(Qt::black),
    m_drawMinMax(false),
    m_resetAllChanges(false)
{
    ui->setupUi(this);
    m_windowSize = QSize(this->geometry().bottomRight().x() - 1, this->geometry().bottomRight().y() - 1);
    m_size = m_windowSize / 2 - QSize(indent, indent);
}

MinMaxDrawDialog::~MinMaxDrawDialog()
{
    delete ui;
}

void MinMaxDrawDialog::setScale(qreal scale)
{
    m_scale = scale;
    update();
}

void MinMaxDrawDialog::setTextColor(QColor color)
{
    m_textColor = color;
    update();
}

void MinMaxDrawDialog::setLinesColor(QColor color)
{
    m_linesColor = color;
    update();
}

QSize MinMaxDrawDialog::sizeHint() const
{
    return m_windowSize;
}

void MinMaxDrawDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.translate(indent, indent);
    painter.translate(m_size.width(), m_size.height());
    painter.setRenderHint(QPainter::Antialiasing);
    //coordinate plain painting and set ordinary coordinate plain except of toQtY()
    painter.setPen(Qt::red);//oY
    painter.drawLine(QPoint(0, toQtY(-m_size.height())), QPoint(0, toQtY(m_size.height())));//axis
    painter.drawLine(QPoint(0, toQtY(m_size.height())), QPoint(-3, toQtY(m_size.height() - 5)));//arrow
    painter.drawLine(QPoint(0, toQtY(m_size.height())), QPoint(3, toQtY(m_size.height() - 5)));//arrow
    painter.drawStaticText(QPoint(-5, toQtY(m_size.height() + 20)), QStaticText("Y"));
    painter.setPen(Qt::blue);//oX
    painter.drawLine(QPoint(-m_size.width(), toQtY(0)), QPoint(m_size.width(), toQtY(0)));//axis
    painter.drawLine(QPoint(m_size.width(), toQtY(0)), QPoint(m_size.width() - 5, toQtY(-3)));//arrow
    painter.drawLine(QPoint(m_size.width(), toQtY(0)), QPoint(m_size.width() - 5, toQtY(3)));//arrow
    painter.drawStaticText(QPoint(m_size.width() + 5, toQtY(10)), QStaticText("X"));
    painter.setClipRect(QRect(QPoint(-m_size.width(), toQtY(m_size.height())),
                              QPoint(m_size.width(), toQtY(-m_size.height()))));

    //text font
    painter.setFont(QFont(QApplication::font().family(), 7));
    painter.save();
    painter.scale(m_scale, m_scale);
    painter.setPen(Qt::lightGray);
    //grid
    qreal scale_diff = 1 / m_scale;
    quint16 gridStep = 10;
    for(quint16 itr = 1; gridStep * itr < m_size.width() * scale_diff; itr++)//vertical
    {
        painter.drawLine(QPoint(itr * gridStep, toQtY(-m_size.height() * scale_diff)),
                         QPoint(itr * gridStep, toQtY(m_size.height() * scale_diff)));
        painter.drawLine(QPoint(-itr * gridStep, toQtY(-m_size.height() * scale_diff)),
                         QPoint(-itr * gridStep, toQtY(m_size.height() * scale_diff)));
    }
    for(quint16 itr = 1; gridStep * itr < m_size.height() * scale_diff; itr++)//horizontal
    {
        painter.drawLine(QPoint(-m_size.width() * scale_diff, toQtY(itr * gridStep)),
                         QPoint(m_size.width() * scale_diff, toQtY(itr * gridStep)));
        painter.drawLine(QPoint(-m_size.width() * scale_diff, toQtY(-itr * gridStep)),
                         QPoint(m_size.width() * scale_diff, toQtY(-itr * gridStep)));
    }
    //end of coordinate plain painting
    painter.setPen(m_linesColor); 
    foreach(const GraphicElement *itr, m_whatToDrawList)
        switch(itr->m_ElementType)
        {
        case GraphicElement::Line:
            const DrawLine *Line = dynamic_cast<const DrawLine *>(itr);
            if(Line->a == 0 && Line->b == 0)
            {
                qDebug() << "a == 0 and b == 0 wtf?? it will be deleted now";
                m_whatToDrawList.removeOne(const_cast<GraphicElement *>(itr));
                //такого не может быть
            }
            else if(Line->a == 0)
            {
                painter.setPen(m_linesColor);
                painter.drawLine(QPointF(-m_size.width() * scale_diff,
                                        toQtY((-1) * Line->c / Line->b)),
                                 QPointF(m_size.width() * scale_diff,
                                        toQtY((-1) * Line->c / Line->b)));
                painter.restore();
                painter.save();
                painter.setPen(m_textColor);
                painter.drawStaticText(QPointF(0,
                                              toQtY((-1) * Line->c / Line->b * m_scale)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->b), 'f', 1)));
                painter.scale(m_scale, m_scale);
            }
            else if(Line->b == 0)
            {
                painter.setPen(m_linesColor);
                painter.drawLine(QPointF((-1) * Line->c / Line->a,
                                        toQtY(-m_size.height()) * scale_diff),
                                 QPointF((-1) * Line->c / Line->a,
                                        toQtY(m_size.height()) * scale_diff));
                painter.restore();
                painter.save();
                painter.setPen(m_textColor);
                painter.drawStaticText(QPointF((-1) * Line->c / Line->a * m_scale,
                                              toQtY(0)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->a), 'f', 1)));
                painter.scale(m_scale, m_scale);
            }
            else
            {
                painter.setPen(m_linesColor);
                painter.drawLine(QPointF((-1) * (Line->c + Line->b * m_size.height() * scale_diff) / Line->a,
                                         toQtY(m_size.height() * scale_diff)),
                                 QPointF((-1) * (Line->c + Line->b * -m_size.height() * scale_diff) / Line->a,
                                         toQtY(-m_size.height() * scale_diff)));
                painter.restore();
                painter.save();
                painter.setPen(m_textColor);
                painter.drawText(QPointF(0, toQtY((-1) * Line->c / Line->b * m_scale)),
                                 QString::number((-1) * Line->c / Line->b, 'f', 1));
                painter.drawText(QPointF((-1) * Line->c / Line->a * m_scale, toQtY(0)),
                                 QString::number((-1) * Line->c / Line->a, 'f', 1));
                painter.scale(m_scale, m_scale);
            }
            break;
        }
    if(m_drawMinMax)
    {
        qreal c = (-1) * Zfunction.a * m_minLineUserAnswer.first.x() -
                Zfunction.b * toQtY(m_minLineUserAnswer.first.y());
        painter.restore();
        painter.save();
        painter.setPen(Qt::magenta);
        painter.drawEllipse(m_minLineUserAnswer.first * m_scale, 5, 5);
        painter.scale(m_scale, m_scale);
        painter.drawLine(QPointF((-1) * (c + Zfunction.b * m_size.height() * scale_diff) / Zfunction.a,
                                 toQtY(m_size.height() * scale_diff)),
                         QPointF((-1) * (c + Zfunction.b * -m_size.height() * scale_diff) / Zfunction.a,
                                 toQtY(-m_size.height() * scale_diff)));

        c = (-1) * Zfunction.a * m_maxLineUserAnswer.first.x() -
                Zfunction.b * toQtY(m_maxLineUserAnswer.first.y());
        painter.restore();
        painter.save();
        painter.setPen(Qt::darkCyan);
        painter.drawEllipse(m_maxLineUserAnswer.first * m_scale, 5, 5);
        painter.scale(m_scale, m_scale);
        painter.drawLine(QPointF((-1) * (c + Zfunction.b * m_size.height() * scale_diff) / Zfunction.a,
                                 toQtY(m_size.height() * scale_diff)),
                         QPointF((-1) * (c + Zfunction.b * -m_size.height() * scale_diff) / Zfunction.a,
                                 toQtY(-m_size.height() * scale_diff)));
    }

    painter.restore();//it generaly hasn't any sense but it needs to avoid multiply warnings about not restored painter save
}

QLineF MinMaxDrawDialog::getQLine(DrawLine Line)
{
    if(Line.a == 0 && Line.b == 0)
    {
        qDebug() << "a == 0 and b == 0 wtf??";
        return QLineF(QPointF(0, 0),
                      QPointF(0, 0));
    }
    else if(Line.a == 0)
    {
        return QLineF(QPointF(-m_size.width(),
                                toQtY((-1) * Line.c / Line.b * m_scale)),
                         QPointF(m_size.width(),
                                toQtY((-1) * Line.c / Line.b * m_scale)));
    }
    else if(Line.b == 0)
    {
        return QLineF(QPointF((-1) * Line.c / Line.a * m_scale,
                                toQtY(-m_size.height())),
                         QPointF((-1) * Line.c / Line.a * m_scale,
                                toQtY(m_size.height())));
    }
    else
    {
        return QLineF(QPointF((-1) * (Line.c + Line.b * m_size.height()) / Line.a,
                                 toQtY(m_size.height())),
                         QPointF((-1) * (Line.c + Line.b * -m_size.height()) / Line.a,
                                 toQtY(-m_size.height())));
    }
}

QLineF MinMaxDrawDialog::getOrdinaryLine(DrawLine Line)
{
    if(Line.a == 0 && Line.b == 0)
    {
        return QLineF(QPointF(0, 0),
                      QPointF(0, 0));
    }
    else if(Line.a == 0)
    {
        return QLineF(QPointF(-10, (-1) * Line.c / Line.b),
                      QPointF(10, (-1) * Line.c / Line.b));
    }
    else if(Line.b == 0)
    {
        return QLineF(QPointF((-1) * Line.c / Line.a, -10),
                      QPointF((-1) * Line.c / Line.a, 10));
    }
    else
    {
        return QLineF(QPointF((-1) * (Line.c + Line.b * 10) / Line.a, 10),
                     QPointF((-1) * (Line.c + Line.b * 20) / Line.a, 20));
    }
}

QLinkedList<QPair<QPointF, QPointF> > MinMaxDrawDialog::findSolutionPolygon(QLinkedList<GraphicElement *> *drawList)
{
    Q_ASSERT(drawList->size() >= 2);
    QLinkedList<DrawLine> allLines;
    //oX and oY axes
    allLines << DrawLine(1, 0, 0);
    allLines << DrawLine(0, 1, 0);
    //collect all lines
    foreach(GraphicElement *itr, *drawList)
    {
        switch(itr->m_ElementType)
        {
        case GraphicElement::Line:
            const DrawLine *Line = dynamic_cast<const DrawLine *>(itr);
            if(Line->a == 0 && Line->b == 0)
            {
                qDebug() << "a == 0 and b == 0 wtf??";
                drawList->removeOne(const_cast<GraphicElement *>(itr));
                //такого не может быть
            }
            else
                allLines << DrawLine(Line->a, Line->b, Line->c);
            break;
        }
    }
    QLinkedList<QPair<QPointF, QPointF> > pointList;//one is scaled point for drawing and another is original for checking
    //collect all intersect points
    for(QLinkedList<DrawLine>::const_iterator itr = allLines.begin(); itr != (allLines.end() - 1); ++itr)
        for(QLinkedList<DrawLine>::const_iterator itr2 = itr + 1; itr2 != allLines.end(); ++itr2)
        {
            QPair<QPointF, QPointF> currentInterceptPoints;
            if(getQLine(*itr).intersect(getQLine(*itr2), &currentInterceptPoints.first) != QLineF::NoIntersection &&
                    getOrdinaryLine(*itr).intersect(getOrdinaryLine(*itr2), &currentInterceptPoints.second) != QLineF::NoIntersection)
            {
                    pointList << currentInterceptPoints;
//                    qDebug() << currentInterceptPoints.first << " || " << currentInterceptPoints.second;
            }
        }
    QLinkedList<QPair<QPointF, QPointF> > result;
    //substitude all points to each equation and exclude point if equaction will NOT be >= 0
    //pop out axes which we used early
    allLines.removeFirst();
    allLines.removeFirst();
    for(QLinkedList<QPair<QPointF, QPointF> >::iterator itrPoint = pointList.begin(); itrPoint != pointList.end(); ++itrPoint)
    {
        bool isNeedToAdd = true;
        foreach(const DrawLine Line, allLines)
            if(qRound((Line.a * (*itrPoint).second.x() + Line.b * (*itrPoint).second.y() + Line.c) * 1000000) / 1000000 < 0 ||
                    (*itrPoint).second.x() < 0 ||
                    (*itrPoint).second.y() < 0)
            {
//                qDebug() << "break" << Line.a << Line.b << Line.c << " = " <<
//                            (Line.a * qRound((*itrPoint).second.x() + Line.b * (*itrPoint).second.y() + Line.c) * 1000) / 1000 <<
//                            " ||| " << (*itrPoint).first << " || " << (*itrPoint).second;
                isNeedToAdd = false;
                break;
            }
        if(isNeedToAdd)
            result << *itrPoint;
    }
    return result;
}

void MinMaxDrawDialog::findMin()
{
    QLinkedList<QPair<QPointF, QPointF> >::iterator itr = m_solutionPoints.begin();
    qreal min = Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c;
    m_minLineAnswer = (*itr);
    itr++;
    for(; itr != m_solutionPoints.end(); ++itr)
        if(min > Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c)
        {
            min = Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c;
            m_minLineAnswer = (*itr);
        }
}

void MinMaxDrawDialog::findMax()
{
    QLinkedList<QPair<QPointF, QPointF> >::iterator itr = m_solutionPoints.begin();
    qreal max = Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c;
    m_maxLineAnswer = (*itr);
    itr++;
    for(; itr != m_solutionPoints.end(); ++itr)
        if(max < Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c)
        {
            max = Zfunction.a * (*itr).second.x() + Zfunction.b * (*itr).second.y() + Zfunction.c;
            m_maxLineAnswer = (*itr);
        }
}

void MinMaxDrawDialog::resetAllChanges()
{
    m_resetAllChanges = true;
    foreach(GraphicElement *itr, m_whatToDrawList)
        itr->~GraphicElement();
    m_whatToDrawList.clear();
    m_scale = 1.0;
    m_drawMinMax = false;
}

void MinMaxDrawDialog::drawTheProblem(double **array, quint8 rowsCount)
{
    resetAllChanges();
    for(quint8 currentRow = 0; currentRow < rowsCount - 1; ++currentRow)
    {
        drawLine(array[currentRow][0], array[currentRow][1], array[currentRow][2]);
    }
    Zfunction = DrawLine(array[rowsCount - 1][0], array[rowsCount - 1][1], array[rowsCount - 1][2]);
    m_solutionPoints = findSolutionPolygon(&m_whatToDrawList);
    findMin();
    findMax();
    nextMin();
    nextMax();
    m_drawMinMax = true;
    update();
    m_resetAllChanges = false;
}

void MinMaxDrawDialog::nextMin()
{
    static QLinkedList<QPair<QPointF, QPointF> >::iterator itr = m_solutionPoints.begin();
    if(m_resetAllChanges)
        itr = m_solutionPoints.begin();
    if(itr == m_solutionPoints.end())
        itr = m_solutionPoints.begin();
    m_minLineUserAnswer = *itr;
//    qDebug() << "nextMin: " << *itr;
    itr++;
    update();
}

void MinMaxDrawDialog::nextMax()
{
    static QLinkedList<QPair<QPointF, QPointF> >::iterator itr = m_solutionPoints.begin();
    if(m_resetAllChanges)
        itr = m_solutionPoints.begin();
    if(itr == m_solutionPoints.end())
        itr = m_solutionPoints.begin();
    m_maxLineUserAnswer = *itr;
//    qDebug() << "nextMax: " << *itr;
    itr++;
    update();
}

void MinMaxDrawDialog::next()
{
    if(m_minLineUserAnswer.second != m_minLineAnswer.second)
        emit userAnswerFalse();
    if(m_minLineUserAnswer.second != m_minLineAnswer.second)
        emit userAnswerFalse();
    emit result(m_minLineAnswer.second, m_maxLineAnswer.second);
}

qreal MinMaxDrawDialog::toQtY(qreal Y)
{
    return -Y;
}

qreal MinMaxDrawDialog::fromQtY(qreal Y)
{
    return -Y;
}

void MinMaxDrawDialog::drawLine(const double a, const double b, const double c)
{
    DrawLine *element = new DrawLine(a, b, c);
    m_whatToDrawList << element;
    update();
}
