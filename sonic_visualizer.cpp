#include "sonic_visualizer.h"
#include <QtWidgets>
#include <QString>



sonic_visualizer::sonic_visualizer(){

    this->setFrameStyle(0);
    this->setStyleSheet("background-color:rgb(250, 250, 250)");
    d = (this->height()-5-5)/20;

}

void sonic_visualizer::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(Qt::blue, 0.5));
    painter.setRenderHint(QPainter::Antialiasing);


    // sonic wave path-------------------------------------------------------------
    sonic_wave_path.moveTo(20,20);
    sonic_wave_path.lineTo(25,50);
    sonic_wave_path.lineTo(30,60);
    sonic_wave_path.lineTo(45,80);
    sonic_wave_path.lineTo(50,100);
    sonic_wave_path.lineTo(55,120);
    sonic_wave_path.lineTo(60,20);
    sonic_wave_path.lineTo(75,50);
    sonic_wave_path.lineTo(80,60);
    sonic_wave_path.lineTo(85,80);
    sonic_wave_path.lineTo(90,100);
    sonic_wave_path.lineTo(95,120);

    painter.drawPath(sonic_wave_path);

    // sonic wave path------------------------------------------------------------
    painter.setPen(QPen(Qt::black, 0.5));
    // vertical line
    painter.drawLine(QPoint(20, 5), QPoint(20, 5+20*d));
    // ticks
    for(int i = 0; i < 21; ++i)
    {
        if(i % 2 == 1)
        {
            painter.drawLine(QPoint(20-3, d*i+5), QPoint(20, d*i+5));
        }
        else
        {
            painter.drawLine(QPoint(20-5, d*i+5), QPoint(20, d*i+5));
        }
    }
    // tick labels
    QFont font = painter.font();
    font.setPixelSize(8);
    painter.setFont(font);
    for(int i = 0; i < 21; ++i)
    {
        painter.drawText( QRect(1, d*i, 14, d), QString("").number(1.0-i*0.1));
    }
    // horizontal line
    painter.setPen(QPen(Qt::blue,0.5));
    painter.drawLine(QPoint(20, 10*d+5), QPoint(this->width(), 10*d+5));
    // central vertical line
    painter.setPen(QPen(Qt::black,2));
    painter.drawLine(QPoint(this->width()/2, 5), QPoint(this->width()/2, 5+20*d));

    paint(painter);
    painter.end();


}

void sonic_visualizer::paint(QPainter &p){
    Q_UNUSED(p);


}

void sonic_visualizer::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    d = (this->height()-5-5)/20;
    update();
}
