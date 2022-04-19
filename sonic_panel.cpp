#include "sonic_panel.h"
#include <QtWidgets>
#include <QString>


sonic_panel::sonic_panel(){

    this->setFrameStyle(0);
    this->setStyleSheet("background-color:rgba(250, 0, 0, 20)");
    d = (this->height()-5-5)/20;
}

void sonic_panel::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(Qt::blue, 0.5));
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::black, 0.5));
    // vertical line
    painter.drawLine(QPoint(22, h/2-d*10), QPoint(22, h/2+d*10));
    // ticks
    d = (h-5-5)/20;
    for(int i = 0; i <= 10; ++i)
    {
        if(i == 0)
        {
            painter.drawLine(QPoint(22-5, h/2), QPoint(22, h/2));
        }
        else
        {
            if(i % 2 == 1)
            {
                painter.drawLine(QPoint(22-3, h/2+d*i), QPoint(22, h/2+d*i));
                painter.drawLine(QPoint(22-3, h/2-d*i), QPoint(22, h/2-d*i));
            }
            else
            {
                painter.drawLine(QPoint(22-5, h/2+d*i), QPoint(22, h/2+d*i));
                painter.drawLine(QPoint(22-5, h/2-d*i), QPoint(22, h/2-d*i));
            }
        }
    }
    // draw text -- tick labels
    QFont font = painter.font();
    font.setPixelSize(8);
    painter.setFont(font);
    for(int i = 0; i < 21; ++i)
    {
        painter.drawText( QRect(1, h/2-10*d+d*i-5, 14, d), Qt::AlignRight, QString("").number(1.0 - i*0.1) );
    }
    // horizontal line
    painter.setPen(QPen(Qt::blue,0.5));
    painter.drawLine(QPoint(22, h/2), QPoint(w, h/2));
    // central vertical line
    painter.setPen(QPen(Qt::black,2));
    painter.drawLine(QPoint(w/2, 5), QPoint(w/2, h-5));

    paint(painter);
    painter.end();


}

void sonic_panel::paint(QPainter &p){
    Q_UNUSED(p);


}

void sonic_panel::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    w = this->width();
    h = this->height();
    qDebug()<< "sonic panel:" << this->height();
    update();
}




