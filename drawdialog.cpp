#include "drawdialog.h"
#include "ui_drawdialog.h"
#include <QStaticText>

DrawDialog::DrawDialog(const QSize size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_windowSize(size),
    m_scale(1.0)
{
    ui->setupUi(this);
     m_size = m_windowSize / 2 - QSize(indent, indent);
}

DrawDialog::DrawDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawDialog),
    m_scale(1.0)
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

QSize DrawDialog::sizeHint() const
{
    return m_windowSize;
}

void DrawDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.translate(indent, indent);
    painter.translate(m_size.width(), m_size.height());
//    painter.setRenderHint(QPainter::Antialiasing);
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
//    painter.drawStaticText(QPoint(0, toQtY(0)), QStaticText("0"));
    //grid
    painter.setPen(Qt::lightGray);
    painter.setFont(QFont(QApplication::font().family(), 7));
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
    painter.setPen(Qt::black);

    foreach(const GraphicElement *itr, m_whatToDrawList)
        switch(itr->m_ElementType)
        {
        case GraphicElement::Line:
            const DrawLine *Line = dynamic_cast<const DrawLine *>(itr);
            if(Line->a == 0 && Line->b == 0)
            {
                qDebug() << "a == 0 and b == 0 wtf??";
                //такого не может быть
            }
            else if(Line->a == 0)
            {
                painter.drawLine(QPointF(-m_size.width(),
                                        toQtY((-1) * Line->c / Line->b * m_scale)),
                                 QPointF(m_size.width(),
                                        toQtY((-1) * Line->c / Line->b * m_scale)));
                painter.drawStaticText(QPointF(0 - 20,
                                              toQtY((-1) * Line->c / Line->b * m_scale + 9)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->b), 'f', 1)));
            }
            else if(Line->b == 0)
            {
                painter.drawLine(QPointF((-1) * Line->c / Line->a * m_scale,
                                        toQtY(-m_size.height())),
                                 QPointF((-1) * Line->c / Line->a * m_scale,
                                        toQtY(m_size.height())));
                painter.drawStaticText(QPointF((-1) * Line->c / Line->a * m_scale - 20,
                                              toQtY(0 + 11)),
                                       QStaticText(QString::number(((-1) * Line->c / Line->a), 'f', 1)));
            }
            else
            {
//                qDebug() << QPointF((-1) * (Line->c + Line->b * m_size.height()) / Line->a * m_scale,
//                                   toQtY(m_size.height() /** m_scale*/));
//                qDebug() << QPointF((-1) * (Line->c + Line->a * -m_size.height()) / Line->b * m_scale,
//                                   toQtY(-m_size.height() /** m_scale*/));
                float fuckin_diff = 0.0;
                if(m_scale < 1.0)
                    fuckin_diff = m_size.height() - m_size.height() * m_scale;
                else
                    fuckin_diff = 0.0;
                painter.drawLine(QPointF((-1) * (Line->c + Line->b * m_size.height()) / Line->a * m_scale +
                                         (-1) * (Line->c + Line->b * m_size.height()) / Line->a / m_size.height() * fuckin_diff,
                                         toQtY(m_size.height() * m_scale + fuckin_diff)),
                                 QPointF((-1) * (Line->c + Line->b * -m_size.height()) / Line->a * m_scale -
                                         (-1) * (Line->c + Line->b * m_size.height()) / Line->a / m_size.height() * fuckin_diff,
                                         toQtY(-m_size.height() * m_scale - fuckin_diff)));
//                painter.drawLine(QPointF(-Line->c / Line->a - Line->b / Line->a * m_size.height(),
//                                         m_size.height()),
//                                 QPointF(-Line->c / Line->a - Line->b / Line->a * -m_size.height(),
//                                         -m_size.height()));
//                painter.drawStaticText(QPointF(0 * m_scale - 20,
//                                              toQtY((-1) * Line->c / Line->b * m_scale + 9)),
//                                       QStaticText(QString::number(((-1) * Line->c / Line->b), 'f', 1)));
            }
            break;
        }

}

void DrawDialog::drawTheProblem(double **array, quint8 row)
{
}

int DrawDialog::toQtY(int Y)
{
    return -Y;
}

void DrawDialog::drawLine(const double a, const double b, const double c)
{
    DrawLine *element = new DrawLine(a, b, c);
    m_whatToDrawList.push_back(element);
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
    drawLine(7, -2 ,14);
    drawLine(7, 4 ,14);
    drawLine(-4, 4 ,14);
    setScale(ui->doubleSpinBox_4->value());
}
