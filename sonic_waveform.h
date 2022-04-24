#ifndef SONIC_WAVEFORM_H
#define SONIC_WAVEFORM_H

#include <QFrame>
#include <QDebug>

class sonic_waveform: public QFrame
{
public:
    sonic_waveform();
    ~sonic_waveform() = default;

    qreal d = 0;
    qint16 h = 0;
    qint16 w = 0;
    qint16 x = 0;
    qint16 y = 0;
    qint16 counter = 0;

    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
public slots:
    void receive_info_from_timeline(qint16 x, qint16 y, qint16 w, qint16 h);
    void add_pixmaps(int counter);

};

#endif // SONIC_WAVEFORM_H
