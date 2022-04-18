
#include "timeline.h"

#include <QWidget>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>



timeline::timeline(){

    this->setWindowTitle(tr("timeline"));
    this->setWindowIcon(QIcon(":/Icon/timeline_editor.png"));
    this->hide();

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
    axisX->setTickCount(10);
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

    //![5]

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
    chartView->setParent(this);
    layout = new QHBoxLayout;
    layout->addWidget(chartView);
    this->setLayout(layout);




}



