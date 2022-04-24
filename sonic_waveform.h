#ifndef SONIC_WAVEFORM_H
#define SONIC_WAVEFORM_H

#include <QFrame>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <QObject>
#include <QWidget>


class sonic_waveform: public QFrame
{
    Q_OBJECT
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
    void mouseMoveEvent(QMouseEvent *e);
signals:
    void send_mouse_pos_to_timeline(int x);

public slots:
    void receive_info_from_timeline(qint16 x, qint16 y, qint16 w, qint16 h);
    void add_pixmaps(int counter);
private:

};

#endif // SONIC_WAVEFORM_H
