#include "sonic_waveform.h"
#include "g_variable.h"

#include <QFile>
#include <QPainter>



sonic_waveform::sonic_waveform(){
    w = this->height();
    h = this->width();

    label_a = new QLabel();
    label_a->setParent(this);

    label_b = new QLabel();
    label_b->setParent(this);
}

void sonic_waveform::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e)
    w = this->width();
    h = this->height();
}

void sonic_waveform::paintEvent(QPaintEvent *e){
    Q_UNUSED(e)

    label_a->resize(w,  h);
    label_a->move(-w/2, 0);
    label_a->setStyleSheet("background-color:rgba(100,50,50,50)");
    label_a->show();

    label_b->resize(w, h);
    label_b->move(w/2, 0);
    label_b->setStyleSheet("background-color:rgba(50,100,50,50)");
    label_b->show();
}





