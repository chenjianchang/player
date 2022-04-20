#ifndef SONIC_WAVEFORM_H
#define SONIC_WAVEFORM_H

#include <QGraphicsScene>
#include <QGraphicsView>

class sonic_waveform: public QGraphicsScene
{
public:
    sonic_waveform();

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void draw_audio_picture(QString path);

    qreal d = 0;

};

#endif // SONIC_WAVEFORM_H
