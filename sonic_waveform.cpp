#include "sonic_waveform.h"
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






//    FILE *fp = fopen("C:/Users/riben/Desktop/player/doc/audio.pcm", "rb+");
//    //    short pcm_In = 0;
//    int size = 0;
//    int counter = 0;
//    short a[500];
//    short x=0;
//    short tempx = 0;
//    short tempy = 0;
//    h = this->height();
//    d = (this->height()-5-5-15)/20;
//    float c = (h-5-5-15)/2+5;


//    while(!feof(fp))
//    {

//        size = fread(&a, 2, 500, fp);
//        if (size > 0)
//        {

//    //        qDebug() << a[0];
//            counter += 1;
//            x += 1;
//            for (int j = 0; j < 500/2; ++j){
//                this->addLine(tempx, tempy, x, c-a[j]/1000*d);
//                tempx = x;
//                tempy = c-a[j]/1000*d;
//            }
//            qDebug() << "c:" << h << ":" << d;


//        }

//    }
//    qDebug() << "counter" << counter;


//    fclose(fp);



