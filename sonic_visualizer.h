#ifndef SONIC_VISUALIZER_H
#define SONIC_VISUALIZER_H

#include <QWidget>
#include <QColor>
#include <QHash>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QFrame>
#include <QResizeEvent>


class sonic_visualizer:public QFrame

{
    Q_OBJECT
public:
    sonic_visualizer();

    void paint(QPainter &p);
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int d;   // vertical distance for tick
    QPainterPath path;
    QPainterPath horizontal_tick;
};




#endif // SONIC_VISUALIZER_H
