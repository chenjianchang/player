#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QBoxLayout>


class timeline: public QWidget
{
    Q_OBJECT
public:
    timeline();
    ~timeline()= default;

    QLineSeries *series = nullptr;
    QChartView *chartView = nullptr;
    QChart *chart = nullptr;
    QDateTimeAxis *axisX = nullptr;
    QValueAxis *axisY = nullptr;
    QBoxLayout *layout = nullptr;

signals:

public slots:

private:




};

#endif // TIMELINE_H
