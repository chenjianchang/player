#include "sonic_waveform.h"


sonic_waveform::sonic_waveform(){
this->addText("hello");
}

void sonic_waveform::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
}


void sonic_waveform::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e);
}
