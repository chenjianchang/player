
#include "timeline.h"

#include <QWidget>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>
#include <QStringList>


timeline::timeline(){

    this->setWindowTitle(tr("timeline"));
    this->setWindowIcon(QIcon(":/Icon/timeline_editor.png"));
    this->hide();

    // create UI
    current_moment = new QLineEdit(this);
    current_moment->setAlignment(Qt::AlignCenter);
    spacer = new QSpacerItem(10,10, QSizePolicy::Expanding);
    zoom = new QLabel(tr("Zoom:"));
    zoom_box = new QComboBox();
    zoom_box->addItem("0.5");
    zoom_box->addItem("1");
    zoom_box->addItem("2");
    zoom_box->addItem("3");
    zoom_box->addItem("4");

    audio_picture_area = new QScrollArea(this);

    h_layout = new QHBoxLayout;
    v_layout = new QVBoxLayout;
    h_layout->addWidget(current_moment);
    h_layout->addSpacerItem(spacer);
    h_layout->addWidget(zoom);
    h_layout->addWidget(zoom_box);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(audio_picture_area);
    this->setLayout(v_layout);



//    audio_picture_area->setWidget(chartView);
//    chartView->move(0,0);
//    audio_picture_area->move(0,0);

    series = new QLineSeries();

    QFile sunSpots("C:/Users/riben/Desktop/player/doc/sun_spots.txt");

    if (!sunSpots.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }

    QTextStream stream(&sunSpots);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.startsWith("#") || line.startsWith(":"))
            continue;
        QStringList values = line.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        QDateTime momentInTime;
        momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt() , 15));
        series->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
    }
    sunSpots.close();

    //![3]
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Sunspots count (by Space Weather Prediction Center)");
    //![3]

    //![4]
    axisX = new QDateTimeAxis;
    axisX->setTickCount(20);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Sunspots count");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    //![4]
    //!
    //![5]

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(60000,200);






//    layout = new QHBoxLayout;
//    layout->addWidget(chartView);
//    this->setLayout(layout);




}



