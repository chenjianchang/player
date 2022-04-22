#include "functions.h"

#include <QOpenGLFunctions>
#include <cmath>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QTextStream>
#include <QByteArray>
#include <QFileInfo>
#include <QSettings>
#include <QFile>
#include <QProcess>
#include <QCoreApplication>



std::string zfill(std::string s, int n, char c){

    if(int(s.size()) >= n)
    {
        return s;
    }
    else
    {
        std::string temp(n - int(s.size()), c);
        return temp + s;
    }
}

QString change_position_into_time(long position){
    // position is a long number with millisecond unit
    return QString::fromStdString(zfill(std::to_string(long(long(long(position / 1000) / 60) / 60)), 2, '0')
                    + ":" + zfill(std::to_string(long(long(position / 1000) / 60) - long(long(long(position / 1000) / 60) / 60) * 60), 2, '0')
                    + ":" + zfill(std::to_string(long(position / 1000) - long(long(position / 1000) / 60) * 60 - long(long(long(position / 1000) / 60) / 60) * 3600), 2, '0')
                    + "." + zfill(std::to_string(long(position % 1000)), 3, '0'));
}

long change_time_into_position(std::string time){
    // time is a string format like 00:01:03.600, return it with a millisecond unit value
    try
    {
        return std::stol(std::string(time, 0, 2)) * 60 * 60 * 1000 + std::stol(std::string(time, 3, 2)) * 60 * 1000 + std::stol(std::string(time, 6, 2)) * 1000 + std::stol(std::string(time, 9, 3));
    }
    catch (std::exception &e)
    {
        write_logs("functions.cpp", "44", QString("").append("this row has some format problems in the subtitle file!").append("  ").append(QString::fromStdString(time)));
        return 1;
    }
}


QList<QString> adjust_subtitle(QList<QString> list, QList<qint64> &time, QList<QString> &line){
    QList<QString> temp;
    QString line_temp = "";

    time.prepend(0);
    time.prepend(0);

    try {
        for(QList<QString>::iterator i = list.begin(); i != list.end(); i++ ){
            if((*i).contains("-->"))
            {
                temp.append(*i);

                try{
                time.append(static_cast<qint64>(     std::stol(std::string((*i).toStdString(), 0, 2)) * 60 * 60 * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 3, 2)) * 60 * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 6, 2)) * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 9, 3))
                                                )
                            );
                time.append(static_cast<qint64>(     std::stol(std::string((*i).toStdString(), 17, 2)) * 60 * 60 * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 20, 2)) * 60 * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 23, 2)) * 1000 +
                                                     std::stol(std::string((*i).toStdString(), 26, 3))
                                               )
                            );
                }
                catch(QString error)
                {
                    write_logs("functions.cpp", "82", *i);
                }

                line.append(line_temp);
                line_temp = "";

                if (  i == (--(list.end()))    )  line.append(line_temp);
            }
            else
            {
                if(!(((*i).trimmed()).isEmpty()))   // if the *i contain meaningful content, then append it
                {
                    temp.append(*i);

                    line_temp += *i;
                }
                if (  i == (--(list.end()))    )  line.append(line_temp);
            }
        }
    }
    catch (std::exception &e){
        write_logs("functions.cpp", "103", "?");
        return temp;
    }
    return temp;
}

void write_logs(QString filename, QString row, QString content){
    QByteArray temp;
    QDate date;
    QDate current_date = date.currentDate();
    QTime time;
    QTime current_time = time.currentTime();
    temp.append(current_date.toString(Qt::ISODate).toUtf8());
    temp.append(" ");
    temp.append(current_time.toString("hh:mm:ss.zzz").toUtf8());
    temp.append("  ");
    temp.append(filename.toUtf8());
    temp.append("  ");
    temp.append(row.toUtf8());
    temp.append("  ");
    temp.append(content.toUtf8());
    QFile logs("C:/Users/riben/Desktop/player/doc/logs.txt");
    if (!logs.open(QIODevice::Append | QIODevice::Text))
             return;
    logs.write("\n");
    logs.write(temp);
    logs.close();
}

void generate_subtitle(QString video_filename, QList<QString> &primary, QList<QString> &adjusted, QList<qint64> &time, QList<QString> &line){
    QString file_name, subtitle_filename;
    QFileInfo fileinfo;

    primary.clear();
    adjusted.clear();
    time.clear();
    line.clear();

    primary.squeeze();
    adjusted.squeeze();
    line.squeeze();
    time.squeeze();

    file_name = video_filename;   // edited by shaolang
    fileinfo = QFileInfo(file_name);   // edited by shaolang
    file_name.chop(fileinfo.suffix().size());   // edited by shaolang
    subtitle_filename = file_name.append("vtt");   // edited by shaolang
    write_logs("functions.cpp", "145", subtitle_filename);

    QFile subtitle_file(subtitle_filename);

    if (subtitle_file.exists())   // edited by shaolang
    {
        write_logs("functions.cpp", "151", "find available subtitle!");
    }
    else
    {
        write_logs("functions.cpp", "156", "subtitle file not exist");
        return;
    }
    if (!subtitle_file.open(QIODevice::ReadOnly | QIODevice::Text)){   // edited by shaolang
        write_logs("functions.cpp", "161", "subtitle open failed");
        return;
    }

    while (!subtitle_file.atEnd()) {
             QString line = subtitle_file.readLine();
             primary.append(line);
    }

    subtitle_file.close();

    adjusted = adjust_subtitle(primary, time, line);

}

void database_statement(){
//    //1 check and open database
//    QSqlDatabase db;
//    if(QSqlDatabase::contains("qt_sql_default_connection"))
//    {
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    }
//    else
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("C:/Users/riben/Desktop/player/CONFIG.db");
//    }
//    // judge weather database is open
//    if(!db.open())
//    {
//        qDebug() << "Error: Failed to connect database." << db.lastError();
//    }
//    else
//    {
//        // do something
//        qDebug() << "do something";
//    }

//    //2 define a query statement
//    QSqlQuery sql_query(db);

//    //3 create table
//    QString create_sql =
//                        "CREATE TABLE 'student' ("
//                            "'id'	INTEGER NOT NULL UNIQUE,"
//                            "'name'	TEXT NOT NULL,"
//                            "'age'	INTEGER NOT NULL,"
//                            "PRIMARY KEY('id')"
//                        ");";

//    sql_query.prepare(create_sql);
//    if(!sql_query.exec())
//    {
//        qDebug() << "Error: Fail to create table." << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "Table created!";
//    }

//    //4 insert data
//    QString insert_sql = "insert into student values (?, ?, ?)";
//    sql_query.prepare(insert_sql);
//    sql_query.addBindValue(3);
//    sql_query.addBindValue("Wang");
//    sql_query.addBindValue(25);
//    if(!sql_query.exec())
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "inserted Wang!";
//    }

//    insert_sql =
//                "INSERT INTO student (id,name,age)"
//                "VALUES (4, 'Allen', 25);";
//    if(!sql_query.exec(insert_sql))
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "inserted Allen!";
//    }

//    //5 update data
//    QString update_sql = "UPDATE student "
//                         "SET name = :name WHERE id = :id;";
//    sql_query.prepare(update_sql);
//    sql_query.bindValue(":name", "Qt");
//    sql_query.bindValue(":id", 3);
//    if(!sql_query.exec())
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "updated!";
//    }

//    //6 update data
//    QString update_sql =
//            "UPDATE student "
//            "SET name = 'Han', age = 34 "
//            "WHERE id = 3;";
//    sql_query.prepare(update_sql);
//    if(!sql_query.exec())
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "updated!";
//    }

//    //7 select data
//    QString select_sql = "SELECT id, name FROM student WHERE id = 5";
//    if(!sql_query.exec(select_sql))
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        while(sql_query.next())
//        {
//            int id = sql_query.value(0).toInt();
//            QString name = sql_query.value(1).toString();
//            qDebug() << QString("id:%1    name:%2").arg(id).arg(name);
//        }
//    }

//    //8 delete table
//    QString drop_sql = "DROP TABLE student;";
//    if(!sql_query.exec(drop_sql))
//    {
//        qDebug() << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "table student dropped!";
//    }


//    //9 close database
//    db.close();
}

bool maybeSave(){
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        // Save was clicked
        return true;
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        return false;
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        return false;
        break;
    default:
        // should never be reached
        return true;
        break;
    }
}

void update_database(QString table, QList<qint16> coordinates){
    //
    if ( table == "UI_CONFIG"   )
    {
        //1 check and open database
        QSqlDatabase db;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            db = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("C:/Users/riben/Desktop/player/doc/CONFIG.db");
        }
        // judge weather database is open
        if(!db.open())
        {
            qDebug() << "Error: Failed to connect database." << db.lastError();
        }
        else
        {
            // do something
            write_logs("functions.cpp -- update_database()", "359", "database open successfully");
        }
        //2 define a query statement
        QSqlQuery sql_query(db);

        //5 update data
        QString update_sql = "UPDATE UI_CONFIG "
                             "SET player_x = :player_x, player_y = :player_y, player_w = :player_w, player_h = :player_h, "
                             "timeline_x = :timeline_x, timeline_y = :timeline_y, timeline_w = :timeline_w,  timeline_h = :timeline_h, "
                             "editor_x = :editor_x, editor_y = :editor_y, editor_w = :editor_w, editor_h = :editor_h "
                             "WHERE id = 1;";
        sql_query.prepare(update_sql);
        sql_query.bindValue(":player_x", coordinates[0]);
        sql_query.bindValue(":player_y", coordinates[1]);
        sql_query.bindValue(":player_w", coordinates[2]);
        sql_query.bindValue(":player_h", coordinates[3]);
        sql_query.bindValue(":timeline_x", coordinates[4]);
        sql_query.bindValue(":timeline_y", coordinates[5]);
        sql_query.bindValue(":timeline_w", coordinates[6]);
        sql_query.bindValue(":timeline_h", coordinates[7]);
        sql_query.bindValue(":editor_x", coordinates[8]);
        sql_query.bindValue(":editor_y", coordinates[9]);
        sql_query.bindValue(":editor_w", coordinates[10]);
        sql_query.bindValue(":editor_h", coordinates[11]);
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            write_logs("functions.cpp -- update_database()", "389", "updated!");
        }
        //9 close database
        db.close();
    }
    //
}

void update_database(QString table, QList<QString> states){
    //
    if ( table == "UI_CONFIG"   )
    {
        //1 check and open database
        QSqlDatabase db;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            db = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("C:/Users/riben/Desktop/player/doc/CONFIG.db");
        }
        // judge weather database is open
        if(!db.open())
        {
            qDebug() << "Error: Failed to connect database." << db.lastError();
        }
        else
        {
            // do something
            write_logs("functions.cpp -- update_database()", "359", "database open successfully");
        }
        //2 define a query statement
        QSqlQuery sql_query(db);

        //5 update data
        QString update_sql = "UPDATE UI_CONFIG SET "
                             "m_subtitle_splitter = :m_subtitle_splitter, "
                             "m_playlist_splitter = :m_playlist_splitter "
                             "WHERE id = 1;";
        sql_query.prepare(update_sql);
        sql_query.bindValue(":m_subtitle_splitter", states[0]);
        sql_query.bindValue(":m_playlist_splitter", states[1]);

        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            write_logs("functions.cpp -- update_database()", "389", "updated!");
        }
        //9 close database
        db.close();
    }
    //
}


void draw_audio_picture(QString path){
    Q_UNUSED(path)
//    FILE *fp = fopen("C:/Users/riben/Desktop/xiaoji/audio.pcm", "rb+");
//    short pcm_In = 0;
//    int size = 0;
//    int counter = 0;
//    short a[500];


//    while(!feof(fp))
//    {

//        size = fread(&a, 2, 500, fp);
//        if (size > 0)
//        {

//            qDebug() << a[0];
//            counter += 1;
//        }

//    }
//    qDebug() << "counter" << counter;
//    qDebug() << "sizeof array" << sizeof(a);

//    fclose(fp);
}

void extract_pcm(QString path){

    // need to configure ffmpeg in the system first
    QProcess _FFMPEG;
    QString _program= "ffmpeg";
    QStringList _command;

//    QString file_name, pcm_filename;
//    QFileInfo fileinfo;

//    file_name = path;   // edited by shaolang
//    fileinfo = QFileInfo(file_name);   // edited by shaolang
//    file_name.chop(fileinfo.suffix().size());   // edited by shaolang
//    pcm_filename = file_name.append("pcm");   // edited by shaolang
//    write_logs("functions.cpp -- extract_pcm()", "490", pcm_filename);

//    if(QFile(pcm_filename).exists())
//    {
//        write_logs("functions.cpp -- extract_pcm()", "498", QString("").append(pcm_filename).append(" already exists!"));
//        return;
//    }


    _command << "-y" << "-i" << path << "-f" << "s16le" << "C:/Users/riben/Desktop/player/temp/introduction.pcm";
    qDebug() << "in extract_pcm";

    _FFMPEG.start(_program, _command);

    if(_FFMPEG.waitForStarted())
    {
        qDebug() << "started successfully!";
    }
    else
    {
        qDebug() << "started failed!";
    }
    while(!_FFMPEG.waitForFinished(1000))
    {
        qDebug() << "wait ... ";
        Sleep(2);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
    }
    qDebug() << "finished";

}
