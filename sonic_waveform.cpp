#include "sonic_waveform.h"


sonic_waveform::sonic_waveform(){
    this->addText("hello");
    this->addLine(1,1,3000,50);

}

void sonic_waveform::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);

}


void sonic_waveform::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e);
}
