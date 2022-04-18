QT += core gui
greaterThan(QT_MAJOR_VERSION, 5):QT += widgets
TEMPLATE = app
TARGET = player

QT += network \
      multimedia \
      multimediawidgets \
      charts \
      sql

HEADERS = \
    editor.h \
    functions.h \
    help.h \
    player.h \
    playercontrols.h \
    playlistmodel.h \
    timeline.h \
    videowidget.h \
    qmediaplaylist.h \
    qmediaplaylist_p.h \
    qplaylistfileparser_p.h

SOURCES = main.cpp \
    editor.cpp \
    functions.cpp \
    help.cpp \
    player.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    timeline.cpp \
    videowidget.cpp \
    qmediaplaylist.cpp \
    qplaylistfileparser.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/player
INSTALLS += target

RESOURCES += \
    Icon.qrc

DISTFILES += \
    doc/CONFIG.db \
    doc/README.md \
    doc/logs.txt \
    doc/sun_spots.txt


