#include "drawdialog.h"
#include "ui_drawdialog.h"
#include <QStaticText>
#include <QLineF>
#include <qmath.h>
#include <QMouseEvent>

DrawDialog::DrawDialog(const QSize size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_windowSize(size),
    m_scale(1.0),
    m_textColor(Qt::red),
    m_linesColor(Qt::black),
    m_firstClickOccured(false)
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
    m_firstClickOccured(false)
{
    ui->setupUi(this);
    m_windowSize = QSize(this->geometry().bottomRight().x() - 1, this->geometry().bottomRight().y() - 1);
    m_size = m_windowSize / 2 - QSize(indent, indent);
}

DrawDialog::~DrawDialog()
{
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
    if(m_firstClickOccured)
    {
        QBrush previousBrush = painter.brush();
        painter.setBrush(QBrush(Qt::yellow));
        painter.drawEllipse(m_userClickPoint, 3, 3);
        painter.setBrush(previousBrush);
    }
}

void DrawDialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
    {
        e->accept();
        return;
    }
    if(!m_firstClickOccured)
    {
        m_firstClickOccured = true;
        emit firstClick();
    }
    QPointF translatedDot = e->posF();
    translatedDot -= QPointF(indent, indent);
    translatedDot -= QPointF(m_size.width(), m_size.height());
    translatedDot /= m_scale;
    m_userClickPoint = translatedDot;//because we use it for drawing, we need it in Qt coordinate system
    translatedDot.ry() *= fromQtY(1);
    m_userAnswer = ifDotIsSolution(translatedDot);
    qDebug() << m_userAnswer;
    update();
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

void DrawDialog::next()
{
    if(!m_userAnswer)
        emit userAnswerFalse();
}

qreal DrawDialog::toQtY(qreal Y)
{
    return -Y;
}

qreal DrawDialog::fromQtY(qreal Y)
{
    return -Y;
}

void DrawDialog::drawLine(const double a, const double b, const double c)
{
    DrawLine *element = new DrawLine(a, b, c);
    m_whatToDrawList << element;
    update();
}

bool DrawDialog::ifDotIsSolution(QPointF clickedDot)
{
    foreach(GraphicElement *itr, m_whatToDrawList)
        if(itr->m_ElementType == GraphicElement::Line)
        {
            DrawLine *Line = dynamic_cast<DrawLine *>(itr);
            if(Line->a * clickedDot.x() + Line->b * clickedDot.y() + Line->c < 0)
                return false;
        }
    return true;
}

DrawLine::DrawLine(double _a, double _b, double _c)  :
    GraphicElement(Line),
    a(_a),
    b(_b),
    c(_c)
{
}
