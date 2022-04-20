#include "sonic_waveform.h"
#include <QFile>
#include <QSvgGenerator>
#include <QPainterPath>



sonic_waveform::sonic_waveform(){

addText("hello world");

}

void sonic_waveform::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);


}


void sonic_waveform::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e);
    d = (this->height()-5-5-15)/20;
    h = this->height();

}



void sonic_waveform::draw_audio_picture(QString path){
    Q_UNUSED(path)
//    glClearColor(0.8, 0.8, 0.8, 0.8);
//    glClear(GL_COLOR_BUFFER_BIT);

//    glLineWidth(2);
//    glBegin(GL_LINES);
//    glColor3f(1.0, 0.0, 0.0);

//    short pcm_In = 0;
//    int size = 0;
//    float xstart = -1.0;
//    float ystart = 0.0;
//    float xend = 0.0;
//    float yend = 0.0;
//    float temp = 0.0;

//    FILE *fp = fopen("C:/Users/riben/Desktop/xiaoji/audio.pcm", "rb+");

//    while(!feof(fp))
//    for(int i = 0; i < 100000; ++i)
//    {
//        size = fread(&pcm_In, 2, 1, fp);
//        if (size > 0)
//        {
//            // waveform points
//            xstart = xstart + 0.00003;
//            ystart = temp;
//            xend = xstart + 0.00003;

//            //
//            yend = (float) pcm_In/10000;

//            //
//            glVertex2f(xstart, ystart);
//            glVertex2f(xend, yend);

//            temp = yend;
//            if(pcm_In!=0)
//                qDebug() << pcm_In;
//        }
//    }

//    fclose(fp);
//    glEnd();

}

