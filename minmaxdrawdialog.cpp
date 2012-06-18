#include "minmaxdrawdialog.h"
#include "ui_minmaxdrawdialog.h"
#include <QStaticText>
#include <QLineF>
#include <qmath.h>
#include <vector>
#include <QDebug>
#include "SolvedSystemDialog.h"

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
    painter.setPen(Qt::green);//oY
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
                m_whatToDrawList.removeOne(const_cast<GraphicElement *>(itr));
                //С‚Р°РєРѕРіРѕ РЅРµ РјРѕР¶РµС‚ Р±С‹С‚СЊ
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
                drawList->removeOne(const_cast<GraphicElement *>(itr));
                //С‚Р°РєРѕРіРѕ РЅРµ РјРѕР¶РµС‚ Р±С‹С‚СЊ
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
        if(isNeedToAdd && !result.contains((*itrPoint)))
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
//    m_solutionPoints.clear();
    m_scale = 1.0;
    m_drawMinMax = false;
}

void MinMaxDrawDialog::drawTheProblem(double **array, quint8 rowsCount)
{
    qreal
    resetAllChanges();
    for(quint8 currentRow = 0; currentRow < rowsCount - 1; ++currentRow)
        drawLine(array[currentRow][0], array[currentRow][1], array[currentRow][2]);
    Zfunction = DrawLine(array[rowsCount - 1][0], array[rowsCount - 1][1], array[rowsCount - 1][2]);

    QLinkedList<QPair<QPointF, QPointF> > solutionPoints = findSolutionPolygon(&m_whatToDrawList);
    for(QLinkedList<QPair<QPointF, QPointF> >::iterator itr = solutionPoints.begin();
        itr != solutionPoints.end(); ++itr)
        qDebug() << (*itr);
    std::vector<std::pair<double, double> > solutionStdVector1;
    for(QLinkedList<QPair<QPointF, QPointF> >::iterator itr = solutionPoints.begin();
        itr != solutionPoints.end(); ++itr)
        solutionStdVector1.push_back(std::make_pair((*itr).second.x(), (*itr).second.y()));
    for(quint8 itr1 = 0; itr1 < rowsCount; itr1++)
            qDebug() << array[itr1][0] << array[itr1][1] << array[itr1][2];
    qDebug() << "std1 size" << solutionStdVector1.size();
    for(std::vector<std::pair<double, double> >::iterator itrVector = solutionStdVector1.begin();
        itrVector != solutionStdVector1.end();
        ++itrVector)
        qDebug() << (*itrVector).first << (*itrVector).second;
    std::vector<std::pair<double, double> > solutionStdVector = ordering(solutionStdVector1, array, rowsCount);
    qDebug() << "std size " << solutionStdVector.size();
    for(std::vector<std::pair<double, double> >::iterator itrVector = solutionStdVector.begin();
        itrVector != solutionStdVector.end();
        ++itrVector)
    {
//        QLinkedList<QPair<QPointF, QPointF> >::iterator itrQVector = solutionPoints.begin();
//        while((*itrQVector).second.x() != (*itrVector).first &&
//              (*itrQVector).second.y() != (*itrVector).second)
//            ++itrQVector;
//        m_solutionPoints << (*itrQVector);
//        solutionPoints.erase(itrQVector);
        qDebug() << (*itrVector).first << (*itrVector).second;
    }
    for(QLinkedList<QPair<QPointF, QPointF> >::iterator itr = m_solutionPoints.begin();
        itr != m_solutionPoints.end();
        ++itr)
        qDebug() << (*itr);
    findMin();
    findMax();
    nextMin();
    nextMax();
    m_drawMinMax = true;
    update();
    m_resetAllChanges = false;
}

std::vector<std::pair<double, double> > MinMaxDrawDialog::ordering(std::vector<std::pair<double, double> > points, double **oldKoef, int n)
{
//    qDebug() << "points: ";
//    for(std::vector<std::pair<double, double> >::iterator itrVector = points.begin();
//        itrVector != points.end();
//        ++itrVector)
//        qDebug() << (*itrVector).first << (*itrVector).second;
//    n += 2;
//    double **koef = new double *[n];
//    for (int i = 0; i < n; i++)
//        koef[i] = new double [3];
//    for (int i = 0; i < n - 3; i++)
//        for (int j = 0; j < 3; j++)
//            koef[i][j] = oldKoef[i][j];
//    koef[n - 3][0] = 1; koef[n - 3][1] = 0; koef[n - 3][2] = 0;
//    koef[n - 2][0] = 0; koef[n - 2][1] = 1; koef[n - 2][2] = 0;
//    koef[n - 1][0] = oldKoef[n - 3][0]; koef[n - 1][1] = oldKoef[n - 3][1]; koef[n - 1][2] = oldKoef[n - 3][2];

//    int indexPoint = 1;
//    int indexLine;

//    for (int i = 0; i < n - 1; i++)
//        if (!(koef[i][0] * points[0].first + koef[i][1] * points[0].second + koef[i][2]))
//        {
//            indexLine = i;
//            break;
//        }
//    qDebug() << "koef array: ";
//    for(int itr = 0; itr < n; itr++)
//        qDebug() <<  koef[itr][0] << koef[itr][1] << koef[itr][2];
//    while (indexPoint != points.size() - 1)
//    {
//        bool check = false;
//        for (int i = indexPoint; i < points.size(); i++)
//            if (!(koef[indexLine][0] * points[i].first + koef[indexLine][1] * points[i].second + koef[indexLine][2]))
//            {
//                std::pair<double, double> temp = points[indexPoint];
//                points[indexPoint] = points[i];
//                points[i] = temp;
//                indexPoint++;
//                check = true;
//                break;
//            }
//        if (!check)
//        {
//            std::vector<std::pair<double, double> > re;
//            qDebug() << points[indexPoint].first << points[indexPoint].second;
//             qDebug() <<  koef[indexLine][0] << koef[indexLine][1] << koef[indexLine][2];
//            return re;
//        }
//        for (int i = 0; i < n - 1; i++)
//            if ((i != indexLine) && !(koef[i][0] * points[indexPoint - 1].first + koef[i][1] * points[indexPoint - 1].second + koef[i][2]))
//            {
//                indexLine = i;
//                break;
//            }
//    }
//    for (int i = 0; i < n; i++)
//        delete [] koef[i];
//    delete [] koef;

//    return points;
    n += 2;
    double **koef = new double *[n];  //Создание нового массива с уравнениями координатных осей
    for (int i = 0; i < n; i++)
        koef[i] = new double [3];
    for (int i = 0; i < n - 3; i++)
        for (int j = 0; j < 3; j++)
            koef[i][j] = oldKoef[i][j];
    koef[n - 3][0] = 1; koef[n - 3][1] = 0; koef[n - 3][2] = 0;
    koef[n - 2][0] = 0; koef[n - 2][1] = 1; koef[n - 2][2] = 0;
    koef[n - 1][0] = oldKoef[n - 3][0]; koef[n - 1][1] = oldKoef[n - 3][1]; koef[n - 1][2] = oldKoef[n - 3][2];

    int indexPoint = 1; //индекс следающая перемещаемой точки
    int indexLine;  // индекс прямой на которой лежит перемещаемая точка

    qDebug() << "koefs 1: ";
    for (int i = 0; i < n - 1; i++)  //нахождение прямой на которой лежит первая точка
    {
        qDebug() << koef[i][0] * points[0].first + koef[i][1] * points[0].second + koef[i][2] <<
                    (koef[i][0] * points[0].first + koef[i][1] * points[0].second + koef[i][2] == 0) <<
                    SolvedSystemDialog::_round(koef[i][0] * points[0].first + koef[i][1] * points[0].second + koef[i][2]);
        if (!SolvedSystemDialog::_round(koef[i][0] * points[0].first + koef[i][1] * points[0].second + koef[i][2]))
        {
            indexLine = i;
            break;
        }
    }

    while (indexPoint != points.size()) //пока следующая перемещаяемая точка не последняя
    {
        bool check = false;
        qDebug() << "koef2 :";
        for (int i = indexPoint; i < points.size(); i++)
        {
            qDebug() << koef[indexLine][0] * points[i].first + koef[indexLine][1] * points[i].second + koef[indexLine][2] <<
                        (koef[indexLine][0] * points[i].first + koef[indexLine][1] * points[i].second + koef[indexLine][2] == 0) <<
                        SolvedSystemDialog::_round(koef[indexLine][0] * points[i].first + koef[indexLine][1] * points[i].second + koef[indexLine][2]);
            if (!SolvedSystemDialog::_round(koef[indexLine][0] * points[i].first + koef[indexLine][1] * points[i].second + koef[indexLine][2]))
            {
                std::pair<double, double> temp = points[indexPoint];
                points[indexPoint] = points[i];
                points[i] = temp;
                indexPoint++;
                check = true;
                break;
            }
        }
        if (!check)
        {
            std::vector<std::pair<double, double> > re;
            return re;
        }
        qDebug() << "koef3 :";
        for (int i = 0; i < n - 1; i++)
        {
            qDebug() << koef[i][0] * points[indexPoint - 1].first + koef[i][1] * points[indexPoint - 1].second + koef[i][2] <<
                        (koef[i][0] * points[indexPoint - 1].first + koef[i][1] * points[indexPoint - 1].second + koef[i][2] == 0) <<
                        SolvedSystemDialog::_round(koef[i][0] * points[indexPoint - 1].first + koef[i][1] * points[indexPoint - 1].second + koef[i][2]);
            if ((i != indexLine) && !SolvedSystemDialog::_round(koef[i][0] * points[indexPoint - 1].first + koef[i][1] * points[indexPoint - 1].second + koef[i][2]))
            {
                indexLine = i;
                break;
            }
        }
    }
    for (int i = 0; i < n; i++)
        delete []koef[i];
    delete []koef;
    return points;
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
    itr++;
    update();
}

bool MinMaxDrawDialog::check()
{
    bool flag = true;
    if(m_minLineUserAnswer.second != m_minLineAnswer.second)
    {
        flag = false;
        emit userAnswerFalse();
    }
    if(m_minLineUserAnswer.second != m_minLineAnswer.second)
    {
        flag = false;
        emit userAnswerFalse();
    }
    if(flag)
        emit result(m_minLineAnswer.second, m_maxLineAnswer.second,
                    m_minLineAnswer.second.x() * Zfunction.a +
                    m_minLineAnswer.second.y() * Zfunction.b + Zfunction.c,
                    m_maxLineAnswer.second.x() * Zfunction.a +
                    m_maxLineAnswer.second.y() * Zfunction.b + Zfunction.c);
    return flag;
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
