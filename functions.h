#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

#include <QString>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QThread>
#include <QPainter>


std::string zfill(std::string s, int n, char c);

QString change_position_into_time(long position);

long change_time_into_position(std::string time);

QList<QString> adjust_subtitle(QList<QString> list, QList<qint64> &time, QList<QString> &line);

void write_logs(QString filename, QString row, QString content);

void generate_subtitle(QString video_filename, QList<QString> &primary, QList<QString> &adjusted, QList<qint64> &time, QList<QString> &line);

void database_statement();

bool maybeSave();

void update_database(QString table, QList<qint16> coordinates);

void update_database(QString table, QList<QString> states);

int draw_audio_picture(QString pcm_path, qint16 w, qint16 h, qreal d);

QString extract_pcm(QString video_path);

class generate_pcm: public QThread{
    Q_OBJECT
public:
    generate_pcm(QString video_path);
    ~generate_pcm()=default;

    void run() override {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        result = extract_pcm(path);
        emit resultReady(result);
    }

signals:
    void resultReady(QString s);

public:
    QString path;
};

class generate_pix: public QThread{
    Q_OBJECT
public:
    generate_pix(QString pcm_path, qint16 width, qint16 height);
    ~generate_pix()=default;

    void run() override {
        int result;
        /* ... here is the expensive or blocking operation ... */
        result = draw_audio_picture(path,w,h,d);
        emit resultReady(result);
    }

signals:
    void resultReady(int s);

public:
    QString path;
    QPixmap pixmap;
    qint16 w=0; // sonic_waveform width
    qint16 h=0; // sonic_waveform height
    qreal d=0; // ticks interval

    QPainter painter;
};

#endif // FUNCTIONS_H



