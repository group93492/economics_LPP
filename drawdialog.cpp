#include "drawdialog.h"
#include "ui_drawdialog.h"
#include <QStaticText>

DrawDialog::DrawDialog(const QSize size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_windowSize(size),
    m_scale(1.0),
    m_shift(shiftX, shiftY)
{
    ui->setupUi(this);
    m_size = QSize(m_windowSize.width() - m_shift.x(),
                   m_windowSize.height() - m_shift.y());
}

DrawDialog::DrawDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_scale(1.0),
    m_shift(shiftX, shiftY)
{
    ui->setupUi(this);
    m_windowSize = QSize(this->geometry().bottomRight().x() - 1, this->geometry().bottomRight().y() - 1);
    m_size = QSize(m_windowSize.width() - m_shift.x(),
                   m_windowSize.height() - m_shift.y());
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

QSize DrawDialog::sizeHint() const
{
    return m_windowSize;
}

void DrawDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.translate(0, m_windowSize.height());
    painter.setRenderHint(QPainter::Antialiasing);
    //coordinate plain painting and set ordinary coordinate plain except of toQtY()
    painter.setPen(Qt::red);//oY
    painter.drawLine(m_shift + QPoint(0, toQtY(0)), m_shift + QPoint(0, toQtY(m_size.height())));//axis
    painter.drawLine(m_shift + QPoint(0, toQtY(m_size.height())), m_shift + QPoint(-3, toQtY(m_size.height() - 5)));//arrow
    painter.drawLine(m_shift + QPoint(0, toQtY(m_size.height())), m_shift + QPoint(3, toQtY(m_size.height() - 5)));//arrow
    painter.drawStaticText(m_shift + QPoint(-5, toQtY(m_size.height() + 20)), QStaticText("Y"));
    painter.setPen(Qt::blue);//oX
    painter.drawLine(m_shift + QPoint(0, toQtY(0)), m_shift + QPoint(m_size.width(), toQtY(0)));//axis
    painter.drawLine(m_shift + QPoint(m_size.width(), toQtY(0)), m_shift + QPoint(m_size.width() - 5, toQtY(-3)));//arrow
    painter.drawLine(m_shift + QPoint(m_size.width(), toQtY(0)), m_shift + QPoint(m_size.width() - 5, toQtY(3)));//arrow
    painter.drawStaticText(m_shift + QPoint(m_size.width() + 5, toQtY(10)), QStaticText("X"));
    painter.drawStaticText(m_shift + QPoint(0, toQtY(0)), QStaticText("0"));
    painter.translate(m_shift);
    //grid
    painter.setPen(Qt::lightGray);
    quint16 step = 10;
    for(quint16 itr = 1; step * itr < m_size.width(); itr++)
        painter.drawLine(QPoint(itr * step, toQtY(0)),
                         QPoint(itr * step, toQtY(m_size.height())));
    for(quint16 itr = 1; step * itr < m_size.height(); itr++)
        painter.drawLine(QPoint(0, toQtY(itr * step)),
                         QPoint(m_size.width(), toQtY(itr * step)));
    //end of coordinate plain painting
    painter.setPen(Qt::black);

    foreach(const GraphicElement *itr, m_whatToDrawVector)
        switch(itr->m_ElementType)
        {
        case GraphicElement::Line:
            const DrawLine *Line = dynamic_cast<const DrawLine *>(itr);
            if(Line->a == 0 && Line->b == 0)
            {
                qDebug() << "a == 0 and b == 0 wtf??";
                //такого не может быть
            }
            else if(Line->c == 0)
            {
                //проходит через начало координат
                if(Line->a == 0)//ось Ox
                    painter.drawLine(QPoint(0, toQtY(0)), QPoint(m_size.width(), toQtY(0)));
                else if(Line->b == 0)//ось Oy
                    painter.drawLine(QPoint(0, toQtY(0)), QPoint(0, toQtY(m_size.height())));
                else//прямая проходит через начало координат
                {
                    if((-1) * Line->a / Line->b > 0)//прямая в 1 четверти
                        painter.drawLine(QPoint(0 * m_scale, toQtY(0 * m_scale)),
                                     QPoint(m_size.width() * m_scale,
                                            toQtY((-1) * Line->a / Line->b * m_size.width() * m_scale)));
                }
            }
            else if(Line->a == 0)
            {
                painter.drawLine(QPoint(0 * m_scale, toQtY((-1) * Line->c / Line->b * m_scale)),
                                 QPoint(m_size.width() * m_scale,
                                        toQtY((-1) * Line->c / Line->b * m_scale)));
                painter.drawStaticText(QPoint(0 * m_scale - 50,
                                              toQtY((-1) * Line->c / Line->b * m_scale + 9)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->b), 'f', 1)));
            }
            else if(Line->b == 0)
            {

            }
            break;
        }

}

int DrawDialog::toQtY(int Y)
{
    return -Y;
}

void DrawDialog::drawLine(const double a, const double b, const double c)
{
    DrawLine *element = new DrawLine(a, b, c);
    m_whatToDrawVector.push_back(element);
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
    drawLine(0, -21, 200);
}

void DrawDialog::on_pushButton_clicked()
{
    drawLine(-5, 5, 0);
}
