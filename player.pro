QT += core gui
greaterThan(QT_MAJOR_VERSION, 5):QT += widgets
TEMPLATE = app
TARGET = player

QT += network \
      multimedia \
      multimediawidgets \
      charts \
      sql \
      svg \
      svgwidgets

HEADERS = \
    editor.h \    
    functions.h \
    g_variable.h \
    help.h \
    player.h \
    playercontrols.h \
    playlistmodel.h \
    sonic_panel.h \
    sonic_waveform.h \
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
    sonic_panel.cpp \
    sonic_waveform.cpp \
    timeline.cpp \
    videowidget.cpp \
    qmediaplaylist.cpp \
    qplaylistfileparser.cpp

DEFINES += SRC_PATH=$${PWD}

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/player
INSTALLS += target

RESOURCES += \
    Icon.qrc

DISTFILES += \
    doc/CONFIG.db \
    doc/README.md \
    doc/logs.txt \

QMAKE_CXXFLAGS +=  -Wno-unused-parameter

INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += -L$$PWD/"ffmpeg/lib" -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale




