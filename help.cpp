#include "help.h"

#include <QStyle>
#include <QIcon>
#include <QBoxLayout>
#include <QTime>


help::help()
{
    this->setWindowTitle(tr("help"));
    this->setWindowIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
    this->hide();

    tabs = new QTabWidget();

    page_one = new QWidget();
    page_two = new QWidget();
    page_three = new QWidget();

    tabs->addTab(page_one,tr("about"));
    tabs->addTab(page_two, tr("options"));

    tabs->addTab(page_three, tr("logs"));
    logs = new QPlainTextEdit();
    QBoxLayout *logs_layout = new QVBoxLayout;
    logs_layout->addWidget(logs);
    logs->setParent(page_three);
    page_three->setLayout(logs_layout);

    logs->appendPlainText("hello");
    logs->appendPlainText("world!");

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabs);
    this->setLayout(layout);

}

