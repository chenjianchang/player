#include "timeline.h"
#include "functions.h"

#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QDebug>


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

    sonic_panel_frame = new sonic_panel();

    sonic_waveform_frame = new sonic_waveform();
    sonic_waveform_frame->setParent(this);
    sonic_waveform_frame->show();

    slider = new QSlider(Qt::Horizontal);

    h_layout = new QHBoxLayout;
    v_layout = new QVBoxLayout;

    h_layout->addWidget(current_moment);
    h_layout->addSpacerItem(spacer);
    h_layout->addWidget(zoom);
    h_layout->addWidget(zoom_box);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(sonic_panel_frame);
    v_layout->addWidget(slider);
    this->setLayout(v_layout);

    this->sonic_panel_frame->setAttribute(Qt::WA_TransparentForMouseEvents);  // set sonic_panel_frame mouse event
}

void timeline::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    this->sonic_waveform_frame->resize(this->sonic_panel_frame->width(), this->sonic_panel_frame->height());
    this->sonic_waveform_frame->move(this->sonic_panel_frame->x(), this->sonic_panel_frame->y());
    this->sonic_waveform_frame->lower();
}

void timeline::generate_pixmap_slot(QString video_filename){
    Q_UNUSED(video_filename)
    qDebug() << "hello";
}

