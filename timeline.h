#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QToolButton>
#include <QSpacerItem>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QPainter>
#include <QFrame>
#include <QSvgWidget>
#include <QGraphicsView>



#include "sonic_panel.h"
#include "sonic_waveform.h"


class timeline: public QWidget
{
    Q_OBJECT
public:
    timeline();
    ~timeline()= default;


    QBoxLayout *layout = nullptr;
    QLineEdit *current_moment = nullptr;
    QSpacerItem *spacer = nullptr;
    QLabel *zoom = nullptr;
    QComboBox *zoom_box = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QVBoxLayout *v_layout = nullptr;
    QPainter *p = nullptr;

    sonic_panel *sonic_panel_frame = nullptr;

    sonic_waveform *sonic_waveform_scene = nullptr;
    QGraphicsView *audio_waveform_view = nullptr;

    void resizeEvent(QResizeEvent *event);



signals:


public slots:

private:




};

#endif // TIMELINE_H
