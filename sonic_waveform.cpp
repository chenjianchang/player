#include "sonic_waveform.h"
#include "g_variable.h"

#include <QFile>
#include <QPainter>



sonic_waveform::sonic_waveform(){
    this->setMouseTracking(true);
}

void sonic_waveform::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e)
}

void sonic_waveform::paintEvent(QPaintEvent *e){
    Q_UNUSED(e)

        this->resize(w*counter + w, h);
        QPainter painter(this);
        for (int i = 1; i <= counter; i++){
            painter.drawPixmap(w/2+w*(i-1), 0,
                               QPixmap(QString(source_path).append("/temp/").append(QString("").number(i)).append(".png")));
        }
}

void sonic_waveform::add_pixmaps(int counter)
{
    this->counter = counter;
    update();
    qDebug() << "add pixmaps";
}

void sonic_waveform::receive_info_from_timeline(qint16 x, qint16 y, qint16 w, qint16 h){
    this->w = w;
    this->h = h;
    this->x = x;
    this->y = y;
    d = (h-5-5)/2;
}

void sonic_waveform::mouseMoveEvent(QMouseEvent *e){
    emit send_mouse_pos_to_timeline(e->pos().x());
}





