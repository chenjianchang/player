#ifndef SONIC_VISUALIZER_H
#define SONIC_VISUALIZER_H

#include <QWidget>
#include <QColor>
#include <QHash>
#include <QFrame>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>


class sonic_panel:public QFrame

{
    Q_OBJECT
public:
    sonic_panel();

    void paint(QPainter &p);
    void change_d();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int w;   // this->width()
    int h;   // this->heigth()
    int d;

};


#endif // SONIC_VISUALIZER_H
