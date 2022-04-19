
#include "timeline.h"


#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>
#include <QStringList>
#include <QPushButton>


timeline::timeline(){

    this->setWindowTitle(tr("timeline"));
    this->setWindowIcon(QIcon(":/Icon/timeline_editor.png"));
    this->hide();

    // create UI
    current_moment = new QLineEdit(this);
    current_moment->setParent(this);
    current_moment->setAlignment(Qt::AlignCenter);
    current_moment->setFixedWidth(150);
    current_moment->setText("current moment");
    spacer = new QSpacerItem(10,10, QSizePolicy::Expanding);
    zoom = new QLabel(tr("Zoom:"));
    zoom->setParent(this);
    zoom_box = new QComboBox();
    zoom_box->addItem("0.5");
    zoom_box->addItem("1");
    zoom_box->addItem("2");
    zoom_box->addItem("3");
    zoom_box->addItem("4");

    audio_picture_frame = new sonic_panel();

    audio_waveform_scene = new sonic_waveform();
    audio_waveform_view = new QGraphicsView(audio_waveform_scene);
    audio_waveform_view->setParent(this);
    audio_waveform_view->show();




    h_layout = new QHBoxLayout;
    v_layout = new QVBoxLayout;
    h_layout->addWidget(current_moment);
    h_layout->addSpacerItem(spacer);
    h_layout->addWidget(zoom);
    h_layout->addWidget(zoom_box);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(audio_picture_frame);
    this->setLayout(v_layout);


}

void timeline::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    this->audio_waveform_view->resize(this->audio_picture_frame->width(), this->audio_picture_frame->height());
    this->audio_waveform_view->move(this->audio_picture_frame->x(), this->audio_picture_frame->y());
    this->audio_waveform_view->lower();

}



