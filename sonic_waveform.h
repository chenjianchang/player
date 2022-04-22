#ifndef SONIC_WAVEFORM_H
#define SONIC_WAVEFORM_H

#include <QFrame>
#include <QLabel>

class sonic_waveform: public QFrame
{
public:
    sonic_waveform();

    qreal d = 0;
    int h = 0;
    int w = 0;
    QLabel *label_a = nullptr;
    QLabel *label_b = nullptr;

    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

};

#endif // SONIC_WAVEFORM_H
