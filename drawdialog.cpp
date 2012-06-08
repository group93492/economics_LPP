#include "drawdialog.h"
#include "ui_drawdialog.h"
#include <QStaticText>
#include <QLineF>
#include <qmath.h>

DrawDialog::DrawDialog(const QSize size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_windowSize(size),
    m_scale(1.0),
    m_textColor(Qt::red),
    m_linesColor(Qt::black),
    m_solutionPolygon(NULL),
    drawpoly(false)
{
    ui->setupUi(this);
     m_size = m_windowSize / 2 - QSize(indent, indent);
}

DrawDialog::DrawDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_scale(1.0),
    m_textColor(Qt::red),
    m_linesColor(Qt::black),
    m_solutionPolygon(NULL),
    drawpoly(false)
{
    ui->setupUi(this);
    m_windowSize = QSize(this->geometry().bottomRight().x() - 1, this->geometry().bottomRight().y() - 1);
    m_size = m_windowSize / 2 - QSize(indent, indent);
}

DrawDialog::~DrawDialog()
{
    delete m_solutionPolygon;
    delete ui;
}

void DrawDialog::setScale(qreal scale)
{
    m_scale = scale;
    update();
}

void DrawDialog::setTextColor(QColor color)
{
    m_textColor = color;
    update();
}

void DrawDialog::setLinesColor(QColor color)
{
    m_linesColor = color;
    update();
}

QSize DrawDialog::sizeHint() const
{
    return m_windowSize;
}

void DrawDialog::paintEvent(QPaintEvent *e)
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
    //grid
    painter.setPen(Qt::lightGray);
    quint16 gridStep = 10 * m_scale;
    for(quint16 itr = 1; gridStep * itr < m_size.width(); itr++)//vertical
    {
        painter.drawLine(QPoint(itr * gridStep, toQtY(-m_size.height())),
                         QPoint(itr * gridStep, toQtY(m_size.height())));
        painter.drawLine(QPoint(-itr * gridStep, toQtY(-m_size.height())),
                         QPoint(-itr * gridStep, toQtY(m_size.height())));
    }
    for(quint16 itr = 1; gridStep * itr < m_size.height(); itr++)//horizontal
    {
        painter.drawLine(QPoint(-m_size.width(), toQtY(itr * gridStep)),
                         QPoint(m_size.width(), toQtY(itr * gridStep)));
        painter.drawLine(QPoint(-m_size.width(), toQtY(-itr * gridStep)),
                         QPoint(m_size.width(), toQtY(-itr * gridStep)));
    }
    //end of coordinate plain painting
    painter.setPen(m_linesColor);

    //text font
    painter.setFont(QFont(QApplication::font().family(), 7));
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
                painter.drawLine(QPointF(-m_size.width(),
                                        toQtY((-1) * Line->c / Line->b * m_scale)),
                                 QPointF(m_size.width(),
                                        toQtY((-1) * Line->c / Line->b * m_scale)));
                painter.setPen(m_textColor);
                painter.drawStaticText(QPointF(0 - 20,
                                              toQtY((-1) * Line->c / Line->b * m_scale + 9)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->b), 'f', 1)));
            }
            else if(Line->b == 0)
            {
                painter.setPen(m_linesColor);
                painter.drawLine(QPointF((-1) * Line->c / Line->a * m_scale,
                                        toQtY(-m_size.height())),
                                 QPointF((-1) * Line->c / Line->a * m_scale,
                                        toQtY(m_size.height())));
                painter.setPen(m_textColor);
                painter.drawStaticText(QPointF((-1) * Line->c / Line->a * m_scale - 20,
                                              toQtY(0 + 11)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->a), 'f', 1)));
            }
            else
            {
                painter.setPen(m_linesColor);
//                painter.drawLine(QPointF((-1) * (Line->c + Line->b * m_size.height()) / Line->a,
//                                         toQtY(m_size.height())),
//                                 QPointF((-1) * (Line->c * (2 * m_scale - 1) + Line->b * -m_size.height()) / Line->a,
//                                         toQtY(-m_size.height()))); //the same but in abbreaviated form
                painter.drawLine(QPointF((-1) * (Line->c + Line->b * m_size.height()) / Line->a,
                                         toQtY(m_size.height())),
                                 QPointF((-1) * (Line->c + Line->b * -m_size.height()) / Line->a * m_scale-
                                         (-1) * (Line->c + Line->b * m_size.height()) / Line->a * (1 - m_scale),
                                         toQtY(-m_size.height())));
                painter.setPen(m_textColor);
                painter.drawText(QPointF(0, toQtY((-1) * Line->c / Line->b * m_scale)),
                                 QString::number((-1) * Line->c / Line->b, 'f', 1));
                painter.drawText(QPointF((-1) * Line->c / Line->a * m_scale, toQtY(0)),
                                 QString::number((-1) * Line->c / Line->a, 'f', 1));
            }
            break;
        }
    if(drawpoly)
    {
        painter.setPen(Qt::green);
        painter.drawPolygon(*findSolutionPolygon(&m_whatToDrawList));
    }
}

QLineF DrawDialog::getQLine(DrawLine Line)
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
                         QPointF((-1) * (Line.c + Line.b * -m_size.height()) / Line.a * m_scale-
                                 (-1) * (Line.c + Line.b * m_size.height()) / Line.a * (1 - m_scale),
                                 toQtY(-m_size.height())));
    }
}

QLineF DrawDialog::getOrdinaryLine(DrawLine Line)
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

void DrawDialog::drawTheProblem(double **array, quint8 rowsCount)
{
    for(quint8 currentRow = 0; currentRow < rowsCount - 1; ++currentRow)
    {
        drawLine(array[currentRow][0], array[currentRow][1], array[currentRow][2]);
    }
}

int DrawDialog::toQtY(int Y)
{
    return -Y;
}

int DrawDialog::fromQtY(int Y)
{
    return -Y;
}

void DrawDialog::drawLine(const double a, const double b, const double c)
{
    DrawLine *element = new DrawLine(a, b, c);
    m_whatToDrawList << element;
    update();
}

QPolygonF *DrawDialog::findSolutionPolygon(QLinkedList<GraphicElement *> *drawList)
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
            if(getQLine(*itr).intersect(getQLine(*itr2), &currentInterceptPoints.first) !=
                    QLineF::NoIntersection &&
                    getOrdinaryLine(*itr).intersect(getOrdinaryLine(*itr2), &currentInterceptPoints.second) !=
                    QLineF::NoIntersection)
                pointList << currentInterceptPoints;
        }
    QPolygonF *resultPolygon = new QPolygonF();
    //substitude all points to each equation and exclude point if equaction will NOT be >= 0
    for(QLinkedList<QPair<QPointF, QPointF> >::iterator itrPoint = pointList.begin(); itrPoint != pointList.end(); ++itrPoint)
    {
        bool isNeedToAdd = true;
        foreach(const DrawLine Line, allLines)
            if(Line.a * (*itrPoint).second.x() + Line.b * (*itrPoint).second.y() + Line.c < 0 ||
                    (*itrPoint).second.x() < 0 || (*itrPoint).second.y() < 0)
            {
                isNeedToAdd = false;
                break;
            }
        if(isNeedToAdd)
        {
//            qDebug() << *itrPoint;
            (*resultPolygon) << (*itrPoint).first;
        }
    }
    qDebug() << *resultPolygon;
    qDebug() << *sortPolygonPointsClockwise(resultPolygon);
    Q_ASSERT(false);
    return resultPolygon;
}

//Находим самую нижнюю (минимальную по Y) точку, если таковых несколько - берём ту, что имеет большую координату по X, то есть, правую точку. Записываем эту точку как A[1].
//    Все остальные точки сортируем по возрастанию угла относительно A[1], если у каких-то точек есть одинаковый угол - то по расстоянию от A[1] (убыванию).
//    Получившиеся точки образуют ломаную без самопересечений, давая нам тот самый "естественный порядок обхода".
QPolygonF *DrawDialog::sortPolygonPointsClockwise(QPolygonF *poly)
{
    QPointF lesser = poly->first();
    for(QVector<QPointF>::iterator point = poly->begin() + 1; point != poly->end(); ++point)
    {
        if(fromQtY(lesser.y()) > fromQtY((*point).y()))
            lesser = (*point);
        else if(fromQtY(lesser.y()) == fromQtY((*point).y()))
            if(lesser.x() < (*point).x())
                lesser = (*point);
    }
    QPolygonF resultPoly;
    resultPoly << lesser;
    qDebug() << "lesser" << lesser;
    QMap<qreal, QPointF> sortedPoints;
    foreach(QPointF point, *poly)
    {
        if(point == lesser)
            continue;
        sortedPoints.insertMulti((lesser.x() * point.x() + fromQtY(lesser.y()) * fromQtY(point.y())) /
                        (qSqrt(qPow(lesser.x(), 2.0) + qPow(fromQtY(lesser.y()), 2.0)) *
                         qSqrt(qPow(point.x(), 2.0) + qPow(fromQtY(point.y()), 2.0))), point);
    }
    for(QMap<qreal, QPointF>::iterator itr = sortedPoints.begin();
        itr != sortedPoints.end();
        ++itr)
    {
        if(sortedPoints.count(itr.key()) > 1)
        {
            QMap<qreal, QPointF> similarAnglePoints;
            //sort by distance from lesser point
            foreach(QPointF point, sortedPoints.values(itr.key()))
                similarAnglePoints.insert(qSqrt(qPow(point.x() - lesser.x(), 2.0) +
                                            qPow(fromQtY(point.y()) - fromQtY(lesser.y()), 2.0)),
                        point);
//            QMap<qreal, QPointF>::iterator itrForReverseCopy = similarAnglePoints.end();
//            while(itrForReverseCopy != similarAnglePoints.begin())
//                    resultPoly << *(--itrForReverseCopy);
            foreach(QPointF point, similarAnglePoints)
                resultPoly << point;
            BUG: extra points
        }
        else
            resultPoly << itr.value();
    }
    (*poly) = resultPoly;
    return poly;
}

void DrawDialog::drawPoly()
{
    drawpoly = true;
    update();
}

DrawLine::DrawLine(double _a, double _b, double _c)  :
    GraphicElement(Line),
    a(_a),
    b(_b),
    c(_c)
{
}

void DrawDialog::on_checkPushButton_clicked()
{
//    drawLine(ui->doubleSpinBox->value(),
//             ui->doubleSpinBox_2->value(),
//             ui->doubleSpinBox_3->value());
    drawLine(-0.8,  0, 8.4);
    drawLine(0, -1.5, 9.75);
    drawLine(-1, -1, 12);
    drawLine(0.8, 1.5, -6.15);
    drawPoly();
    setScale(ui->doubleSpinBox_4->value());
}
