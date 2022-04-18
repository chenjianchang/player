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
#include <QPainter>
#include <QScrollArea>
#include <QLineEdit>
#include <QToolButton>
#include <QSpacerItem>
#include <QLayout>
#include <QLabel>
#include <QComboBox>



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
    QScrollArea *audio_picture_area = nullptr;
    QLineEdit *current_moment = nullptr;
    QSpacerItem *spacer = nullptr;
    QLabel *zoom = nullptr;
    QComboBox *zoom_box = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QVBoxLayout *v_layout = nullptr;


signals:

public slots:

private:




};

#endif // TIMELINE_H
