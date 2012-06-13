#ifndef GRAPHDIALOG2_H
#define GRAPHDIALOG2_H

#include <QWidget>

namespace Ui {
class GraphDialog2;
}

class GraphDialog2 : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphDialog2(QWidget *parent = 0);
    ~GraphDialog2();
    
private:
    Ui::GraphDialog2 *ui;
};

#endif // GRAPHDIALOG2_H
