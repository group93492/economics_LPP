#ifndef DRAWDIALOG_H
#define DRAWDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QLinkedList>

namespace Ui {
class DrawDialog;
}

class GraphicElement;
class DrawLine;

class DrawDialog : public QWidget
{
    Q_OBJECT
    
private:
    Ui::DrawDialog *ui;
    QSize m_size;//size of coordinate plain
    QSize m_windowSize;//size of paint window
    qreal m_scale;//size multiplayer for scaling
    QLinkedList<GraphicElement *> m_whatToDrawList;
    QColor m_textColor;
    QColor m_linesColor;
    static qreal toQtY(qreal Y);
    static qreal fromQtY(qreal Y);
    bool m_userAnswer;
    bool m_firstClickOccured;
    QPointF m_userClickPoint;
    enum{
        indent = 20 //отступ в пикселях от краев окна для надписей и т.д.
    };
    void drawLine(const double x, const double y, const double c);
    QLineF getQLine(DrawLine Line);
    QLineF getOrdinaryLine(DrawLine Line);
    bool ifDotIsSolution(QPointF clickedDot);
    QLinkedList<QPair<QPointF, QPointF> > findSolutionPolygon(QLinkedList<GraphicElement *> *drawList);
    void resetAllChanges();
    static std::vector<std::pair<double, double> > Ordering(std::vector<std::pair<double, double> > points, double **oldKoef, int n);

public:
    explicit DrawDialog(QWidget *parent = 0);
    explicit DrawDialog(const QSize size, QWidget *parent = 0);
    ~DrawDialog();
    void setScale(qreal scale);
    void setTextColor(QColor color);
    void setLinesColor(QColor color);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

public slots:
    void drawTheProblem(double **array, quint8 rowsCount);
    bool check();
    void setCorrectAnswer();

signals:
    void firstClick(); //emited after first click on widget
};

class GraphicElement
{
public:
    enum ElementType{
        Line
    };
    ElementType m_ElementType;
    GraphicElement(ElementType type) { m_ElementType = type; }
    virtual ~GraphicElement() {}
};

class DrawLine : public GraphicElement
{
public:
    DrawLine(double _a, double _b, double _c);
    DrawLine() : GraphicElement(Line) {}
    ~DrawLine() {}
    double a;
    double b;
    double c;
};

#endif // DRAWDIALOG_H
