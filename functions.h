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

void draw_audio_picture(QString path);

void extract_pcm(QString path);

class generate_pix: QThread
{
    Q_OBJECT
public:
    generate_pix()=default;
    ~generate_pix()=default;

    void run() override {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &s);

public:
    QString path;
    QPixmap pixmap;
    int w; // sonic_waveform width
    int h; // sonic_waveform height
    int d; // ticks interval

    QPainter painter;
};

#endif // FUNCTIONS_H



