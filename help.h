#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QString>


class help:public QWidget
{
public:
    help();

    QTabWidget *tabs = nullptr;
    QWidget *page_one = nullptr;
    QWidget *page_two = nullptr;
    QWidget *page_three = nullptr;
    QPlainTextEdit *logs = nullptr;




};


#endif // HELP_H
